#include "TaskManager.h"

namespace robot{

void TaskManager::updateStatus(AbstractTask* taskSender, TaskState newState){
    debug("Updating status");
    {
        lock_guard<mutex> lock(heapModification);
        taskCache.at(taskSender->getName()).first.task->setState(newState);
        orderedTasks.increase(taskCache.at(taskSender->getName()).second);
    }
}

bool TaskManager::addTask(AbstractTask* newTask){
    newTask->registerManager(this);
    RankedTask rankedTask;
    rankedTask.task=newTask;
    rankedTask.rank=10;
    {
        lock_guard<mutex> lock(heapModification);
        TaskQueue::handle_type handle=orderedTasks.push(rankedTask);
        CachedRankedTask cachedTask(rankedTask,handle);
        taskCache[newTask->getName()]=cachedTask;
    }
}

bool TaskManager::getWorldProperty() const{
    //TODO
    return false;
}

bool TaskManager::setWorldProperty(){
    //TODO
    return false;
}

bool TaskManager::sendMessage(Message* message){
    messageQueueLock.lock();
    messageQueue.push(message);
    messageQueueLock.unlock();
    messageQueueNotEmpty.notify_one();
    return true;
}

bool TaskManager::receiveMessage(Message* message){
    //TODO: add pipes to quickly process message before sending to other manager
    executorManager->sendMessage(message);
}

void TaskManager::init(){
    for (TaskQueue::ordered_iterator it=orderedTasks.ordered_begin();it!=orderedTasks.ordered_end();++it){
        it->task->init();
    }
}

void TaskManager::stop(){
    //TODO: Stop all tasks
}

void TaskManager::startAllTasks(){
    for (TaskQueue::ordered_iterator it=orderedTasks.ordered_begin();it!=orderedTasks.ordered_end();++it){
        debug("Starting task");
        it->task->start();
    }
}

void TaskManager::main(){
    startAllTasks();
    dispatchMessage();
}

Message* TaskManager::popNextMessage(){
    unique_lock<boost::mutex> lock(messageQueueLock);
    while (messageQueue.empty()) {
        messageQueueNotEmpty.wait(lock);
    }
    Message* message=messageQueue.front();
    messageQueue.pop();
    return message;
}

void TaskManager::setExecutorManager(AbstractMessageHandler *_executorManager){
    executorManager=_executorManager;
}

void TaskManager::dispatchMessage(){
    while(!shouldStop){
        Message* message=popNextMessage();
        switch (message->getMessageType()) {
        case NOTIFICATION:
        {
            lock_guard<mutex> lock(heapModification);
            for (TaskQueue::ordered_iterator it=orderedTasks.ordered_begin();it!=orderedTasks.ordered_end();++it){
                it->task->passMessage(message);
            }
        }
        break;
        case COMMAND_RESPONSE:
            CommandResponse* resp=(CommandResponse*)message;
            string destination=resp->getDestination();
            {
                lock_guard<mutex> lock(heapModification);
                taskCache.at(destination).first.task->passMessage(message);
            }
            break;
        }
    }
}

}
