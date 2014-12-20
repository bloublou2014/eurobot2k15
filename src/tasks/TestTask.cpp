#include "TestTask.h"

namespace robot {

void TestTask::init(){

}

void TestTask::initScript(){
    this->subscribe("milan",(notificationCallback)&TestTask::handleMilanNotifications);
}

void TestTask::startScript(){
    debug("Script is started");
}

void TestTask::stopScript(){

}

void TestTask::handleMilanNotifications(Notification* testNotification){
    debug("Milan notification received");

    MoveForward* cmd=new MoveForward(200);
    sendCommand(cmd,(responseCallback)&TestTask::moveForwardSuccess,
                (responseCallback)&TestTask::motionError);
}

void TestTask::moveForwardSuccess(CommandResponse* resp){
    debug("Moved forward");
    RotateFor* cmd=new RotateFor(180);
    sendCommand(cmd,(responseCallback)&TestTask::rotateForSuccess,
                (responseCallback)&TestTask::motionError);
}

void TestTask::rotateForSuccess(CommandResponse* resp){
    debug("Rotated for success");
    RotateTo* cmd=new RotateTo(90);
    sendCommand(cmd,(responseCallback)&TestTask::rotateToSuccess,
                (responseCallback)&TestTask::motionError);
}

void TestTask::rotateToSuccess(CommandResponse* resp){
    debug("rotate to success");
    MoveArc* cmd=new MoveArc(Point2D(10,10),90);
    sendCommand(cmd,(responseCallback)&TestTask::moveArcSuccess,
                (responseCallback)&TestTask::motionError);
}

void TestTask::moveArcSuccess(CommandResponse* resp){
    debug("move arc success");
    StopMovement* cmd=new StopMovement();
    sendCommand(cmd,(responseCallback)&TestTask::stopMovementSuccess,
                (responseCallback)&TestTask::motionError);
}

void TestTask::stopMovementSuccess(CommandResponse* resp){
    debug("stop movement success");
    MoveToPosition* cmd=new MoveToPosition(Point2D(20,20));
    sendCommand(cmd,(responseCallback)&TestTask::moveForwardSuccess,
                (responseCallback)&TestTask::motionError);
}

void TestTask::motionError(CommandResponse* resp){
    debug("Error in motion");
}

}
