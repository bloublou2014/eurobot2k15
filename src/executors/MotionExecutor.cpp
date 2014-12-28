#include "MotionExecutor.h"

#include <iostream>

using namespace std;

namespace motion {

string MotionExecutor::NAME="MotionExecutor";

//Obavezno koristiti static_cast
void MotionExecutor::init(){
    this->registerCommand(MotionCommand::NAME,static_cast<commandCallback>(&MotionExecutor::processMotionCommand));
    this->registerCommand(GetMotionState::NAME,static_cast<commandCallback>(&MotionExecutor::processGetMotionState));

    motionHandles[MotionCommand::MotionType::MOVE_STRAIGHT]=static_cast<motionCommandHandle>(&MotionExecutor::moveForward);
    motionHandles[MotionCommand::MotionType::MOVE_TO_POSITION]=static_cast<motionCommandHandle>(&MotionExecutor::moveToPosition);
    motionHandles[MotionCommand::MotionType::ROTATE_FOR]=static_cast<motionCommandHandle>(&MotionExecutor::rotateFor);
    motionHandles[MotionCommand::MotionType::ROTATE_TO]=static_cast<motionCommandHandle>(&MotionExecutor::rotateTo);
    motionHandles[MotionCommand::MotionType::MOVE_ARC]=static_cast<motionCommandHandle>(&MotionExecutor::moveArc);
    motionHandles[MotionCommand::MotionType::STOP]=static_cast<motionCommandHandle>(&MotionExecutor::stopMovement);
}

void MotionExecutor::processMotionCommand(Command* command){
    /*Ubacim najnoviju komandu u queue*/
    commandQueueLock.lock();
    commandsToProcess.push(command);
    commandQueueLock.unlock();
}

void MotionExecutor::processGetMotionState(Command* command){
    stateLock.lock();
    GetMotionStateResponse* resp=new GetMotionStateResponse(command->getSource(),command->getDestination(),lastState);
    resp->setId(command->getId());
    stateLock.unlock();
    sendResponse(resp);
}

MotionCommand* MotionExecutor::getNextMotionCommand(){
    MotionCommand* newCommand=NULL;
    commandQueueLock.lock();
    /*Posaljem poruku greske svim porukama osim poslednje i vratim poslednju poruku*/
    if (!commandsToProcess.empty()){
        while(commandsToProcess.size()>1){
            Command* cmd=commandsToProcess.front();
            commandsToProcess.pop();
            debug("Cant process command, have newer, sending error");
            sendResponseFromCommand(cmd,ERROR);
        }
        newCommand=(MotionCommand*)commandsToProcess.front();
        commandsToProcess.pop();
    }
    commandQueueLock.unlock();
    return newCommand;
}

void MotionExecutor::stop(){
    shouldStop=true;
}

void MotionExecutor::main(){
    shouldStop=false;
    debug("Started main thread execution");
    while (true){
        if (shouldStop){
            driver.stop();
            break;
        }

        /*Dobavim sledecu komandu*/
        MotionCommand* newCommand=getNextMotionCommand();
        if (newCommand!=NULL && currentMotionCommand!=NULL){
            debug("Newer command received, sending error to old");
            sendResponseFromCommand(currentMotionCommand,ERROR);
        }
//        if (newCommand!=NULL)
//            currentMotionCommand=newCommand;

        /*Sad bi trebalo tu komandu odraditi*/
        if (newCommand!=NULL){
            (this->*motionHandles[newCommand->getMotionType()])(newCommand);
        }

        /*refresh of driver, verovatno?*/
        driver.refreshData();
        MotionState newState;
        newState.Direction=driver.getDirection();
        newState.Orientation=driver.getOrientation();
        newState.Position=driver.getPosition();
        newState.Speed=driver.getSpeed();
        newState.State=driver.getState();

        if (newState.State==MotionDriver::State::IDLE && currentMotionCommand!=NULL){
            sendResponseFromCommand(currentMotionCommand);
            currentMotionCommand=NULL;
        }

        if ((newState.State==MotionDriver::State::ERROR || newState.State==MotionDriver::State::STUCK) && currentMotionCommand!=NULL){
            sendResponseFromCommand(currentMotionCommand, ERROR);
            currentMotionCommand=NULL;
        }

        if (lastState!=newState){
           stateLock.lock();
           lastState=newState;
           stateLock.unlock();
           MotionNotification* motionNotification=new MotionNotification(newState);
           sendNotification(motionNotification);
        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(10));

    }
    debug("Stopping execution");
}

void MotionExecutor::moveToPosition(MotionCommand* _motionCommand){
    MoveToPosition* command=(MoveToPosition*)_motionCommand;
    debug("Moving to position: ");
    currentMotionCommand=_motionCommand;
    driver.moveToPosition(command->getPosition(),command->getDirection());
}

void MotionExecutor::moveForward(MotionCommand* _motionCommand){
    MoveForward* command=(MoveForward*)_motionCommand;
    debug("Moving forward: ");
    currentMotionCommand=_motionCommand;
    driver.moveStraight(command->getDistance());
}

void MotionExecutor::rotateFor(MotionCommand* _motionCommand){
    RotateFor* command=(RotateFor*)_motionCommand;
    debug("Rotating for: ");
    currentMotionCommand=_motionCommand;
    driver.rotateFor(command->getRelativeAngle());
}

void MotionExecutor::rotateTo(MotionCommand* _motionCommand){
    RotateTo* command=(RotateTo*)_motionCommand;
    debug("Rotating to: ");
    currentMotionCommand=_motionCommand;
    driver.rotateFor(command->getAbsoluteAngle());
}

void MotionExecutor::moveArc(MotionCommand* _motionCommand){
    MoveArc* command=(MoveArc*)_motionCommand;
    debug("Moving arc: ");
    currentMotionCommand=_motionCommand;
    driver.moveArc(command->getCenter(),command->getAngle(),command->getDirection());
}

void MotionExecutor::stopMovement(MotionCommand* _motionCommand){
    debug("Stopping movement: ");
    currentMotionCommand=_motionCommand;
    driver.stop();
}

}
