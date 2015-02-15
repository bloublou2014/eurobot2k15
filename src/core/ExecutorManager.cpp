#include "ExecutorManager.h"

namespace robot{


bool ExecutorManager::getWorldProperty() const{
    return true;
}

bool ExecutorManager::setWorldProperty(){
    return true;
}

bool ExecutorManager::sendMessage(Message* message){
    messageQueueLock.lock();
    messageQueue.push(message);
    messageQueueLock.unlock();
    messageQueueNotEmpty.notify_one();
    return true;
}

bool ExecutorManager::receiveMessage(Message* message){
    //TODO: add pipes to quickly process message before sending to other manager
    return taskManager->sendMessage(message);
}

bool ExecutorManager::addExecutor(AbstractExecutor* newExecutor){
    newExecutor->registerManager(this);
    boost::upgrade_lock<shared_mutex> lock(executorsMapManipulation);
    boost::upgrade_to_unique_lock<shared_mutex> uniqueLock(lock);
    executorsMap[newExecutor->getName()]=newExecutor;

    return true;
}

void ExecutorManager::init(){
    map<string,AbstractExecutor*>::const_iterator it=executorsMap.cbegin();
    for (;it!=executorsMap.cend();++it){
        it->second->init();
    }
}

void ExecutorManager::stop(){
    debug("Stopping executor manager");
    shouldStop=true;
    this->sendMessage(new StopMessage("Executor manager"));
}

void ExecutorManager::main(){
    shouldStop=false;
    startAllExecutors();
    dispatcheMessage();
    debug("*** Finished ***");
}

void ExecutorManager::startAllExecutors(){
    boost::shared_lock<shared_mutex> lock(executorsMapManipulation);
    map<string,AbstractExecutor*>::const_iterator it=executorsMap.cbegin();
    for (;it!=executorsMap.cend();++it){
        debug("Starting executor");
        it->second->start();
    }
}

Message* ExecutorManager::popNextMessage(){
    boost::unique_lock<boost::mutex> lock(messageQueueLock);
    while (messageQueue.empty()) {
        messageQueueNotEmpty.wait(lock);
    }
    Message* message=messageQueue.front();
    messageQueue.pop();
    return message;
}

void ExecutorManager::dispatcheMessage(){
    while(!shouldStop){
        Message* message=popNextMessage();
        if (shouldStop) break;
        switch (message->getMessageType()) {
        case NOTIFICATION:
        {
            boost::shared_lock<shared_mutex> lock(executorsMapManipulation);
            for (map<string,AbstractExecutor*>::const_iterator it=executorsMap.cbegin();it!=executorsMap.cend();++it){
                if (it->second->isSubscribed(message))
                    it->second->processNotification(message->clone());
            }
            delete message;
        }
        break;
        case COMMAND:
        {
            Command* command=(Command*)message;
            map<string,AbstractExecutor*>::iterator destIt;
            boost::shared_lock<shared_mutex> lock(executorsMapManipulation);
            if ((destIt=executorsMap.find(command->getDestination()))!=executorsMap.end()){
                destIt->second->processCommand(command);
            }
        }
        break;
        default:
            break;
        }
    }
    stopAllExecutors();
}

void ExecutorManager::stopAllExecutors(){
    boost::shared_lock<shared_mutex> lock(executorsMapManipulation);
    map<string,AbstractExecutor*>::const_iterator it=executorsMap.cbegin();
    debug("Stopping all executors");
    for (;it!=executorsMap.cend();++it){
        it->second->stop();
    }

    it=executorsMap.cbegin();
    debug("Waiting to join on executors");
    for (;it!=executorsMap.cend();++it){
        it->second->join();
    }
}

void ExecutorManager::setTaskManager(AbstractMessageHandler *_taskManager){
    taskManager=_taskManager;
}

}
