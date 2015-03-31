#include "ServosExecutor.h"

#include <iostream>

using namespace std;

namespace servo{

string ServoExecutor::NAME="ServoExecutor";


void ServoExecutor::init(){
    this->registerCommand(ServoCommand::NAME,static_cast<commandCallback>(&ServoExecutor::processServoCommand));

    servoHandles[ServoCommand::ServoType::ROTATE_FOR]=static_cast<servoCommandHandle>(&ServoExecutor::rotateFor);
    servoHandles[ServoCommand::ServoType::ROTATE_TO_POSITION]=static_cast<servoCommandHandle>(&ServoExecutor::rotateToPosition);
    servoHandles[ServoCommand::ServoType::SET_SPEED]=static_cast<servoCommandHandle>(&ServoExecutor::setSpeed);

    driver.setServoSlaveAddress(char(1));
    driver.setServoPositionAddress(char(1));
    debug("servo address is setted");
}

void ServoExecutor::processServoCommand(Command* command){
    //debug("NewCommand");
    //driver.rotateFor(200);
    commandQueueLock.lock();
    commandsToProcess.push(command);
    commandQueueLock.unlock();
}


ServoCommand* ServoExecutor::getNextServoCommand(){
    ServoCommand* newCommand = NULL;
    commandQueueLock.lock();
    if (!commandsToProcess.empty()){
        while(commandsToProcess.size()>1){
            Command* cmd = commandsToProcess.front();
            commandsToProcess.pop();
            debug("Cant process command, have newer, sending error");
            sendResponseFromCommand(cmd,ERROR);
        }
        newCommand=(ServoCommand*)commandsToProcess.front();
        commandsToProcess.pop();
    }
    commandQueueLock.unlock();
    return newCommand;
}


void ServoExecutor::stop(){
    shouldStop = true;
}

void ServoExecutor::main(){
    shouldStop = false;
    bool run = true;
    while(run){
        ServoCommand* newCommand = getNextServoCommand();
        //debug("executing new command");
        if (newCommand!=NULL && currentServoCommand!= NULL){
            debug("Newer command received, sending error to old");
            sendResponseFromCommand(currentServoCommand, ERROR);
        }

        if (newCommand!=NULL){
            //debug("execute command");
            (this->*servoHandles[newCommand->getServoType()])(newCommand);
        }else{
            if(shouldStop == true){
                run = false;
            }
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }

}

void ServoExecutor::rotateFor(ServoCommand* _servoCommand){
    bool success;
    RotateFor* command=(RotateFor*)_servoCommand;
    debug("rotateFor");
    currentServoCommand = _servoCommand;
    success = driver.rotateFor(command->getSteps());
    if (success) {
        currentServoCommand = NULL;
    }
}

void ServoExecutor::rotateToPosition(ServoCommand* _servoCommand){
    bool success;
    RotateToPosition* cmd=(RotateToPosition*) _servoCommand;
    debug("rotateTo");
    currentServoCommand = _servoCommand;
    success = driver.rotateToPosition(cmd->getPosition());
    if (success){
        currentServoCommand = NULL;
    }

}

void ServoExecutor::setSpeed(ServoCommand* _servoCommand){

    SetSpeed* cmd = (SetSpeed*) _servoCommand;
    currentServoCommand = _servoCommand;
    driver.setServoSpeed(cmd->getSpeed());
    currentServoCommand = NULL;

}

}
