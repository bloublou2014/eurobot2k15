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
    motionHandles[MotionCommand::MotionType::SET_SPEED]=static_cast<motionCommandHandle>(&MotionExecutor::setSpeed);
    motionHandles[MotionCommand::MotionType::SET_POSITION]=static_cast<motionCommandHandle>(&MotionExecutor::setPosition);
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

    int commandCycle=0;
    int maxCommandCycle=5;

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

        //Process received command
        try{
            /*Sad bi trebalo tu komandu odraditi*/
            if (newCommand!=NULL){
                (this->*motionHandles[newCommand->getMotionType()])(newCommand);
                commandCycle=0;
            }
        }catch(...){
            error("***** Error in UART communication! ****");
            sendResponseFromCommand(currentMotionCommand,ERROR);
            return;
        }

        //Update data from dirver
        try{
            driver.refreshData();
        }catch(...){
            error("***** Error in UART communication! ****");
            return;
        }
        MotionState newState;
        newState.Direction=driver.getDirection();
        newState.Orientation=driver.getOrientation();
        newState.Position=driver.getPosition();
        newState.Speed=driver.getSpeed();
        newState.State=driver.getState();

        //If robot finished movement, report it back
        if (newState.State==MotionDriver::State::IDLE && currentMotionCommand!=NULL){
            debug("Robot finished executing command");
            sendResponseFromCommand(currentMotionCommand);
            currentMotionCommand=NULL;
        }

        //If driver throws an error, report it
        if ((newState.State==MotionDriver::State::ERROR || newState.State==MotionDriver::State::STUCK) && currentMotionCommand!=NULL){
            sendResponseFromCommand(currentMotionCommand, ERROR);
            error("Robot stuck, canceling movement");
            currentMotionCommand=NULL;
        }

        //Detect stuck state
        if(commandCycle>maxCommandCycle){
            if (isStuck(previousState, newState)){
                driver.stop();
                error("Robot stuck, stopping movement!");
                sendResponseFromCommand(currentMotionCommand, ERROR);
                currentMotionCommand=NULL;
            }
            commandCycle=0;
            //Update variable for stuck detection
            previousState=newState;
            maxCommandCycle=6*(100.0f/newState.Speed);  //tako sam probao i radi
        }else{
            commandCycle++;
        }

        //Send progress if there is a command for it
        if (lastState!=newState){
            if(currentMotionCommand!=NULL){
                GetMotionStateResponse* progress=new GetMotionStateResponse(currentMotionCommand->getSource(),
                         currentMotionCommand->getDestination(),newState,ResponseStatus::PROGRESS_UPDATE);
                progress->setId(currentMotionCommand->getId());
                sendResponse(progress);
            }

            stateLock.lock();
            lastState=newState;
            stateLock.unlock();
            MotionNotification* motionNotification=new MotionNotification(newState);
            sendNotification(motionNotification);
        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(5));
    }
    debug("Stopping execution");
}

void MotionExecutor::moveToPosition(MotionCommand* _motionCommand){
    MoveToPosition* command=(MoveToPosition*)_motionCommand;
    debug("Moving to position");
    currentMotionCommand=_motionCommand;
    driver.moveToPosition(command->getPosition(),command->getDirection());
}

void MotionExecutor::moveForward(MotionCommand* _motionCommand){
    MoveForward* command=(MoveForward*)_motionCommand;
    debug("Moving forward");
    currentMotionCommand=_motionCommand;
    driver.moveStraight(command->getDistance());
}

void MotionExecutor::rotateFor(MotionCommand* _motionCommand){
    RotateFor* command=(RotateFor*)_motionCommand;
    debug("Rotating for");
    currentMotionCommand=_motionCommand;
    driver.rotateFor(command->getRelativeAngle());
}

void MotionExecutor::rotateTo(MotionCommand* _motionCommand){
    RotateTo* command=(RotateTo*)_motionCommand;
    debug("Rotating to");
    currentMotionCommand=_motionCommand;
    driver.rotateFor(command->getAbsoluteAngle());
}

void MotionExecutor::moveArc(MotionCommand* _motionCommand){
    MoveArc* command=(MoveArc*)_motionCommand;
    debug("Moving arc");
    currentMotionCommand=_motionCommand;
    driver.moveArc(command->getCenter(),command->getAngle(),command->getDirection());
}

void MotionExecutor::setSpeed(MotionCommand* _motionCommand){
    debug("Setting speed");

    SetSpeedMotion* command=(SetSpeedMotion*)_motionCommand;
    currentMotionCommand=_motionCommand;
    std::stringstream ss;
    ss<<"Set speed @ "<<command->getSpeed();
    debug(ss.str());
    driver.setSpeed(command->getSpeed());
}

void MotionExecutor::setPosition(MotionCommand* _motionCommand){
    debug("Setting position");
    SetPosition* command=(SetPosition*)_motionCommand;
    currentMotionCommand=_motionCommand;
    std::stringstream ss;
    debug(ss.str());
    driver.setPositionAndOrientation(command->getPoint(),command->getOrientation());
}


void MotionExecutor::stopMovement(MotionCommand* _motionCommand){
    debug("Stopping movemen");
    currentMotionCommand=_motionCommand;
    driver.stop();
}

double MotionExecutor::distance(double xFirst, double yFirst, double xSecond, double ySecond){
    double x=xFirst-xSecond;
    double y=yFirst-ySecond;
    return sqrt(pow(x,2)+pow(y,2));
}

bool MotionExecutor::isStuck(MotionState& oldState, MotionState& newState){
    if (oldState.State==MotionDriver::State::MOVING && newState.State==MotionDriver::State::MOVING){
        if (distance(oldState.Position.getX(), oldState.Position.getY(),newState.Position.getX(), newState.Position.getY())<2.0)
            return true;
    }
    if (oldState.State==MotionDriver::State::ROTATING && newState.State==MotionDriver::State::ROTATING){
        if (abs(oldState.Orientation-newState.Orientation)<1)
            return true;
    }
    return false;
}

}
