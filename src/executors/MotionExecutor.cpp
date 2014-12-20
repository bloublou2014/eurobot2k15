#include "MotionExecutor.h"

#include <iostream>

using namespace std;

namespace motion {

string MotionExecutor::NAME="MotionExecutor";

//Obavezno koristiti static_cast
void MotionExecutor::init(){
    this->registerCommand(MotionCommand::NAME,static_cast<commandCallback>(&MotionExecutor::processMotionCommand));

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
            //driver.stop();
            break;
        }

        /*Dobavim sledecu komandu*/
        MotionCommand* newCommand=getNextMotionCommand();
        if (newCommand!=NULL && currentMotionCommand!=NULL){
            debug("Newer command received, sending error to old");
            sendResponseFromCommand(currentMotionCommand,ERROR);
        }
        if (newCommand!=NULL)
            currentMotionCommand=newCommand;

        /*Sad bi trebalo tu komandu odraditi*/
        if (currentMotionCommand!=NULL){
            (this->*motionHandles[currentMotionCommand->getMotionType()])(currentMotionCommand);
        }

        /*refresh of driver, verovatno?*/
        //driver.refreshData();
         boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
    debug("Stopping execution");
}

void MotionExecutor::moveToPosition(MotionCommand* _motionCommand){
    MoveToPosition* command=(MoveToPosition*)_motionCommand;
    debug("Moving to position: ");
    sendResponseFromCommand(currentMotionCommand,ERROR);
    currentMotionCommand=NULL;
}

void MotionExecutor::moveForward(MotionCommand* _motionCommand){
    MoveForward* command=(MoveForward*)_motionCommand;
    debug("Moving forward: ");
    sendResponseFromCommand(currentMotionCommand);
    currentMotionCommand=NULL;
}

void MotionExecutor::rotateFor(MotionCommand* _motionCommand){
    RotateFor* command=(RotateFor*)_motionCommand;
    debug("Rotating for: ");
    sendResponseFromCommand(currentMotionCommand);
    currentMotionCommand=NULL;
}

void MotionExecutor::rotateTo(MotionCommand* _motionCommand){
    RotateTo* command=(RotateTo*)_motionCommand;
    debug("Rotating to: ");
    sendResponseFromCommand(currentMotionCommand);
    currentMotionCommand=NULL;
}

void MotionExecutor::moveArc(MotionCommand* _motionCommand){
    MoveArc* command=(MoveArc*)_motionCommand;
    debug("Moving arc: ");
    sendResponseFromCommand(currentMotionCommand);
    currentMotionCommand=NULL;
}

void MotionExecutor::stopMovement(MotionCommand* _motionCommand){
    debug("Stopping movement: ");
    sendResponseFromCommand(currentMotionCommand);
    currentMotionCommand=NULL;
}

}
