#include "ExampleExecutor.h"

#include <iostream>

using namespace std;

namespace robot {

string ExampleExecutor::NAME="ExampleExecutor";

void ExampleExecutor::init(){
    this->subscribe("probeEvent",(notificationCallback)&ExampleExecutor::processProbeEvent);
    this->registerCommand("countToN",(commandCallback)&ExampleExecutor::countToN);
}

void  ExampleExecutor::processProbeEvent(Notification* notification){
    cout<<"Received notification! Yea! from: "<<notification->getSource()<<endl;
}

void ExampleExecutor::countToN(Command* command){
    debug("Received command to countdown");
    commandQueueLock.lock();
    commandsToProcess.push(command);
    commandQueueLock.unlock();
}

void ExampleExecutor::stop(){
    shouldStop=true;
}

void ExampleExecutor::main(){
    shouldStop=false;
    cout<<"Started thread"<<endl;
    while (true){
        if (shouldStop) break;
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        counter++;
        if ((counter%5)==0){
            TimePassedNotification* timePassed=new TimePassedNotification(getName(),5);
            sendNotification(timePassed);
        }
        Command* newCommand=ExecuteNextCommand();
        if (newCommand!=NULL && currentCommand!=NULL){
            debug("Newer command received, sending error to old");
            sendResponseFromCommand(currentCommand,ERROR);
        }
        if (newCommand!=NULL)
            currentCommand=newCommand;
        if (currentCommand!=NULL){
            debug("Sending progress update");
            sendResponseFromCommand(currentCommand,PROGRESS_UPDATE);
        }
    }
    debug("Stopping execution");
}

Command* ExampleExecutor::ExecuteNextCommand(){
    Command* newCommand=NULL;
    commandQueueLock.lock();
    if (!commandsToProcess.empty()){
        while(commandsToProcess.size()>1){
            Command* cmd=commandsToProcess.front();
            commandsToProcess.pop();
            debug("Cant process command, have newer, sending error");
            sendResponseFromCommand(cmd,ERROR);
        }
        newCommand=commandsToProcess.front();
        commandsToProcess.pop();
    }
    commandQueueLock.unlock();
    return newCommand;
}

}
