#include "TaskManager.h"

const int TaskManager::matchDuration=90;

namespace robot{

TaskManager::TaskManager(const string& strategy, const string &scheduler, const string& directory):Node("TaskManager"),shouldStop(false),
    matchStarted(false), matchFinished(false), scheduler(scheduler,this){
    using namespace boost::property_tree;
    ptree pt;

    read_xml(strategy,pt, boost::property_tree::xml_parser::no_comments);
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("tasks"))
            createTask(v.second.get<std::string>("name"),
                       v.second.get<int>("rank",0), directory, v.second.get<int>("finalize",0));
}

void TaskManager::createTask(const string& name, int rank, const string& directory, bool finalize){
    string filename=name+".js";
    debug(directory+boost::filesystem::path::preferred_separator+filename);
    JavaScriptTask* task=new JavaScriptTask(name,directory+boost::filesystem::path::preferred_separator+filename,directory,rank);
    task->registerManager(this);
    addTask(task);
}

bool TaskManager::updateStatus(const string &taskName, TaskState newState){
    TaskState oldState=availableTasks[taskName]->getTaskState();
    availableTasks[taskName]->setState(newState);   //Save state to task
    if (matchStarted){  //Don't bother scheduler before match has started
        scheduler.updateTaskState(availableTasks.at(taskName), oldState, newState);   //notify scheduler of event
    }
    return true;
}

StartMessage::Color TaskManager::getMatchColor(){
    return matchColor;
}

bool TaskManager::isMatchStarted() const{
    return matchStarted;
}

bool TaskManager::addTask(AbstractTask *task){
    availableTasks[task->getName()]=task;
    scheduler.addTask(task);
    return true;
}

string TaskManager::getWorldProperty(const string& key){
    boost::mutex::scoped_lock lock(worldStateLock);
    map<string,string>::iterator it;
    if ((it=worldState.find(key))!=worldState.end()){
        return it->second;
    }else{
        return "";
    }
}

void TaskManager::setWorldProperty(const string& key, const string& value){
    boost::mutex::scoped_lock lock(worldStateLock);
    worldState[key]=value;
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
    map<string, AbstractTask*>::iterator it=availableTasks.begin();
    for (;it!=availableTasks.end();++it){
        it->second->init();
    }
    scheduler.init();
}

void TaskManager::stop(){
    debug("Stopping task manager");
    shouldStop=true;
    this->sendMessage(new StopMessage("Task Manager"));
}

void TaskManager::startAllTasks(){
    debug("Starting all tasks");
    map<string, AbstractTask*>::iterator it=availableTasks.begin();
    for (;it!=availableTasks.end();++it){
        it->second->start();
    }
    scheduler.start();
}

void TaskManager::stopAllTasks(){
    debug("Stopping all tasks");
    scheduler.stop();
    map<string, AbstractTask*>::iterator it=availableTasks.begin();
    for (;it!=availableTasks.end();++it){
        it->second->stop();
    }

    debug("Joingn on all tasks");
    scheduler.join();
    it=availableTasks.begin();
    for (;it!=availableTasks.end();++it){
        it->second->join();
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
                map<string, AbstractTask*>::iterator it=availableTasks.begin();
                for (;it!=availableTasks.end();++it){
                    if (it->second->isSubscribed(notification))
                        it->second->passMessage(notification->clone());
                }
                if (scheduler.isSubscribed(message)){
                    scheduler.passMessage(message->clone());
                }
                delete message;
            }
        break;
        case COMMAND_RESPONSE:
            {
                CommandResponse* resp=(CommandResponse*)message;
                string destination=resp->getDestination();
                {
                    availableTasks.at(destination)->passMessage(message);
                }
            }
        break;
        case START_MESSAGE:{
            StartMessage* sm=(StartMessage*)message;
            matchColor=sm->getColor();
            matchStarted=true;
            scheduler.startDispatching();
        }
        break;
        default:
            //Wrong message in queue
            error("Wrong message in queue");
        }
    }
    stopAllTasks();
}

}
