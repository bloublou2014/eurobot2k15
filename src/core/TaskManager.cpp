#include "TaskManager.h"

namespace robot{

TaskManager::TaskManager(const string& strategy, const string& directory):Node("TaskManager"),shouldStop(false),matchStarted(false){
    using namespace boost::property_tree;
    ptree pt;

    read_xml(strategy, pt);
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("tasks"))
            createTask(v.second.get<std::string>("name"),v.second.get<std::string>("filename"),v.second.get<int>("rank"),v.second.get<int>("time"), directory);
}

void TaskManager::createTask(const string& name, const string& filename, int rank, int duration, const string& directory){
    debug(directory+boost::filesystem::path::preferred_separator+filename);
    JavaScriptTask* task=new JavaScriptTask(name,directory+boost::filesystem::path::preferred_separator+filename);
    task->registerManager(this);
    RankedTask rt;
    rt.task=task;
    rt.rank=rank;
    rt.estimatedRunningTime=duration;
    addTask(rt);
}

bool TaskManager::updateStatus(const string &taskName, TaskState newState){
    debug("Updating status");
    //If state is changing from running to something else notify the task to pause
    AbstractTask* task=taskCache.at(taskName).first.task;
    if (task->getTaskState()==TaskState::RUNNING && newState!=TaskState::RUNNING){
        task->pauseTask();
    }
    //Rearange tasks
    {
        lock_guard<mutex> lock(heapModification);
        task->setState(newState);
        orderedTasks.increase(taskCache.at(taskName).second);
    }

    runBestTask();
    return true;
}

bool TaskManager::addTask(RankedTask& rankedTask){
    lock_guard<mutex> lock(heapModification);
    TaskQueue::handle_type handle=orderedTasks.push(rankedTask);
    CachedRankedTask cachedTask(rankedTask,handle);
    taskCache[rankedTask.task->getName()]=cachedTask;
    return true;
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

    return true;
}

void TaskManager::init(){
    for (TaskQueue::ordered_iterator it=orderedTasks.ordered_begin();it!=orderedTasks.ordered_end();++it){
        it->task->init();
    }
}

void TaskManager::stop(){
    debug("Stopping task manager");
    shouldStop=true;
    this->sendMessage(new StopMessage("Task Manager"));
}

void TaskManager::startAllTasks(){
    debug("Starting all tasks");
    for (TaskQueue::ordered_iterator it=orderedTasks.ordered_begin();it!=orderedTasks.ordered_end();++it){
        it->task->start();
    }
}

void TaskManager::stopAllTasks(){
    debug("Stopping all tasks");
    for (TaskQueue::ordered_iterator it=orderedTasks.ordered_begin();it!=orderedTasks.ordered_end();++it){
        it->task->stop();
    }

    debug("Joingn on all tasks");
    for (TaskQueue::ordered_iterator it=orderedTasks.ordered_begin();it!=orderedTasks.ordered_end();++it){
        it->task->join();
    }
}

void TaskManager::main(){
    shouldStop=false;
    startAllTasks();
    dispatchMessage();
    debug("*** Finished ***");
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
        if (shouldStop) break;
        switch (message->getMessageType()) {
        case NOTIFICATION:
            {
                Notification* notification=(Notification*)message;
                lock_guard<mutex> lock(heapModification);
                for (TaskQueue::ordered_iterator it=orderedTasks.ordered_begin();it!=orderedTasks.ordered_end();++it){
                    if (it->task->isSubscribed(notification))
                        it->task->passMessage(notification->clone());
                }
                delete message;
            }
        break;
        case COMMAND_RESPONSE:
            {
                CommandResponse* resp=(CommandResponse*)message;
                string destination=resp->getDestination();
                {
                    lock_guard<mutex> lock(heapModification);
                    taskCache.at(destination).first.task->passMessage(message);
                }
            }
        break;
        case START_MESSAGE:{
            matchStarted=true;
            runBestTask();
        }
        break;
        default:
            //Wrong message in queue
            error("Wrong message in queue");
        }
    }
    stopAllTasks();
}

void TaskManager::runBestTask(){
    debug("Running best task");
    if (!matchStarted) return;
    AbstractTask* task;
    {
        //Get best task
        lock_guard<mutex> lock(heapModification);
        RankedTask bestTask= orderedTasks.top();
        task=bestTask.task;
        string taskName=task->getName();
        //If best task is ready than start it
        if (task->getTaskState()==TaskState::READY){
            task->setState(TaskState::RUNNING);
            orderedTasks.increase(taskCache.at(task->getName()).second);
        }else{
            //If it's not ready or is already running than don't start it
            task=NULL;
        }
    }
    if (task){
        task->runTask();
    }
}

}
