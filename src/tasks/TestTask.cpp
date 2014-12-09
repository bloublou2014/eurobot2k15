#include "TestTask.h"

namespace robot {

void TestTask::init(){

}

void TestTask::initScript(){
    this->subscribe("timePassedNotification",(notificationCallback)&TestTask::handleTimePassNotification);
    this->subscribe("milan",(notificationCallback)&TestTask::handleMilanNotifications);
}

void TestTask::startScript(){
    debug("Script is started");
}

void TestTask::stopScript(){

}

void TestTask::handleTimePassNotification(Notification* notification){
    TimePassedNotification* timeNotification=(TimePassedNotification*)notification;
    debug("Hello, second passed: 5");
}

void TestTask::handleMilanNotifications(Notification* testNotification){
    debug("Milan notification received");
    CountdownCommand* cmd=new CountdownCommand("ExampleExecutor",this->getName());
    sendCommand(cmd,(responseCallback)&TestTask::countdownSuccess,
                (responseCallback)&TestTask::countdownError,
                (responseCallback)&TestTask::countdownProgress);
}

void TestTask::countdownSuccess(CommandResponse* resp){
    debug("SUCCESS: ");
}

void TestTask::countdownProgress(CommandResponse* resp){
    debug("PROGRESS: ");
}

void TestTask::countdownError(CommandResponse* resp){
    debug("ERROR: ");
}

}
