#include "TestTask.h"

namespace robot {

void TestTask::init(){

}

void TestTask::initScript(){
    this->subscribe("milan",(notificationCallback)&TestTask::handleMilanNotifications);
    this->subscribe("MotionNotification",(notificationCallback)&TestTask::handleMotionNotification);
}

void TestTask::startScript(){
    debug("Script is started");
}

void TestTask::stopScript(){

}



//MotionNotification

void TestTask::handleMotionNotification(Notification* resp){
    debug("notification");
    GetMotionStateResponse* stateResponse=(GetMotionStateResponse*)resp;
    MotionState state= stateResponse->getState();

    stringstream sstr;
    sstr<<"position: (";
    sstr<<state.Position.getX();
    sstr<<", ";
    sstr<<state.Position.getY();
    sstr<<"); speed: ";
    sstr<<(int)state.Speed;
    sstr<<" state: ";
    sstr<<state.State;

    debug(sstr.str());
}

void TestTask::handleMilanNotifications(Notification* testNotification){
    debug("Milan notification received");
    //get MOtion state
    GetMotionState* getCmd=new GetMotionState();
    sendCommand(getCmd, (responseCallback)&TestTask::motionStateReceived,
                (responseCallback)&TestTask::motionError);
    //Send motion command
    MoveForward* cmd=new MoveForward(500);
    sendCommand(cmd,(responseCallback)&TestTask::moveForwardSuccess,
                (responseCallback)&TestTask::motionError);
}

void TestTask::moveForwardSuccess(CommandResponse* resp){
    debug("Moved forward");
    RotateFor* cmd=new RotateFor(90);
    sendCommand(cmd,(responseCallback)&TestTask::rotateForSuccess,
                (responseCallback)&TestTask::motionError);
}

void TestTask::rotateForSuccess(CommandResponse* resp){
    debug("Rotated for success");
    MoveForward* cmd=new MoveForward(500);
    sendCommand(cmd,(responseCallback)&TestTask::moveForwardSuccess,
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

void TestTask::motionStateReceived(CommandResponse* resp){
    debug("Motion state received");
    GetMotionStateResponse* stateResponse=(GetMotionStateResponse*)resp;
    MotionState state= stateResponse->getState();

    stringstream sstr;
    sstr<<"position: (";
    sstr<<state.Position.getX();
    sstr<<", ";
    sstr<<state.Position.getY();
    sstr<<"); speed: ";
    sstr<<state.Speed;
    sstr<<" state: ";
    sstr<<state.State;

    debug(sstr.str());
}

}
