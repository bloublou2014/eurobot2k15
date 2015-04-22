#include "TaskManager.h"

namespace robot{

TaskManager::TaskManager(const string& strategy, const string& directory):Node("TaskManager"),shouldStop(false),
    matchStarted(false), currentlyRunningTask(NULL){
    using namespace boost::property_tree;
    ptree pt;

    read_xml(strategy, pt);
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("tasks"))
            createTask(v.second.get<std::string>("name"),v.second.get<std::string>("filename"),v.second.get<int>("rank"),v.second.get<int>("time"), directory);
}

void TaskManager::createTask(const string& name, const string& filename, int rank, int duration, const string& directory){
    debug(directory+boost::filesystem::path::preferred_separator+filename);
    JavaScriptTask* task=new JavaScriptTask(name,directory+boost::filesystem::path::preferred_separator+filename,directory);
    task->registerManager(this);
    RankedTask rt;
    rt.task=task;
    rt.rank=rank;
    rt.estimatedRunningTime=duration;
    addTask(rt);
}

bool TaskManager::updateStatus(const string &taskName, TaskState newState){
    std::stringstream ss;
    ss<<"Updating status "<<taskName<<" State: "<<newState;
    debug(ss.str());
    boost::mutex::scoped_lock lock(tasksLock);
    bool shouldUpdate=false;
    // If running task is changing it's state from ready than a new task must be found
    AbstractTask* task=availableTasks.at(taskName).task;
    if (task->getTaskState()==TaskState::RUNNING && newState!=TaskState::READY){
        shouldUpdate=true;
    }
    if (newState==TaskState::READY){
        shouldUpdate=true;
    }
    task->setState(newState);

    if (shouldUpdate){
        runBestTask();
    }
    return true;
}

StartMessage::Color TaskManager::getMatchColor(){
    return matchColor;
}

bool TaskManager::isMatchStarted() const{
    return matchStarted;
}

bool TaskManager::addTask(RankedTask& rankedTask){
    availableTasks[rankedTask.task->getName()]=rankedTask;
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
    map<string, RankedTask>::iterator it=availableTasks.begin();
    for (;it!=availableTasks.end();++it){
        it->second.task->init();
    }
}

void TaskManager::stop(){
    debug("Stopping task manager");
    shouldStop=true;
    this->sendMessage(new StopMessage("Task Manager"));
}

void TaskManager::startAllTasks(){
    debug("Starting all tasks");
    map<string, RankedTask>::iterator it=availableTasks.begin();
    for (;it!=availableTasks.end();++it){
        it->second.task->start();
    }
}

void TaskManager::stopAllTasks(){
    debug("Stopping all tasks");
    map<string, RankedTask>::iterator it=availableTasks.begin();
    for (;it!=availableTasks.end();++it){
        it->second.task->stop();
    }

    debug("Joingn on all tasks");
    it=availableTasks.begin();
    for (;it!=availableTasks.end();++it){
        it->second.task->join();
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
        switch (message->getMessageType()){
        case NOTIFICATION:
            {
                Notification* notification=(Notification*)message;
                map<string, RankedTask>::iterator it=availableTasks.begin();
                for (;it!=availableTasks.end();++it){
                    if (it->second.task->isSubscribed(notification))
                        it->second.task->passMessage(notification->clone());
                }
                delete message;
            }
        break;
        case COMMAND_RESPONSE:
            {
                CommandResponse* resp=(CommandResponse*)message;
                string destination=resp->getDestination();
                {
                    availableTasks.at(destination).task->passMessage(message);
                }
            }
        break;
        case START_MESSAGE:{
            StartMessage* sm=(StartMessage*)message;
            matchColor=sm->getColor();
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

bool TaskManager::larger(const RankedTask& t1, const RankedTask& t2){
    return t1.rank>t2.rank;
}

//Finds the best task and runs it, if it is already running than it ignores it
void TaskManager::runBestTask(){
    debug("Running best task");
    if (!matchStarted) return;
    map<string, RankedTask>::iterator it=availableTasks.begin();
    map<string, RankedTask>::iterator maxElement=availableTasks.end();
    //Find the best ranked ready or running task
    for (;it!=availableTasks.end();++it){
        if (it->second.task->getTaskState()==TaskState::READY
                || it->second.task->getTaskState()==TaskState::RUNNING){
            if (maxElement!=availableTasks.end()){
                if (larger(it->second, maxElement->second)){
                    maxElement=it;
                }
            }else{
                maxElement=it;
            }
        }
    }

    //Stop the running task and start the best task
    if (maxElement!=availableTasks.end()){
        if ((maxElement->second.task->getTaskState()!=TaskState::RUNNING)){
            if (currentlyRunningTask!=NULL){
                currentlyRunningTask->pauseTask();
            }
            currentlyRunningTask=maxElement->second.task;
            currentlyRunningTask->runTask();
        }
    }else if (maxElement!=availableTasks.end() && currentlyRunningTask!=NULL){
        if (currentlyRunningTask->getTaskState()!=TaskState::RUNNING){
            currentlyRunningTask->pauseTask();
            currentlyRunningTask=NULL;
        }
    }


}

}
