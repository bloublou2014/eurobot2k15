#include "MotionExecutor.h"

#include <iostream>

#define MOTION_TRESHOLD 10

using namespace std;

namespace motion {

string MotionExecutor::NAME="MotionExecutor";
string MotionExecutor::CONFIG_FILENAME="MotionConfig.xml";

//Obavezno koristiti static_cast
void MotionExecutor::init(){
    using namespace boost::property_tree;
    ptree pt;

    read_xml(CONFIG_FILENAME, pt);
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("motionConfig"))
    {
        rBrkon=v.second.get<int>("rBrkon");
        rSensor=v.second.get<int>("rSensor");
        maxX=v.second.get<int>("maxX");
        maxY=v.second.get<int>("maxY");
    }

    this->registerCommand(MotionCommand::NAME,static_cast<commandCallback>(&MotionExecutor::processMotionCommand));
    this->registerCommand(GetMotionState::NAME,static_cast<commandCallback>(&MotionExecutor::processGetMotionState));
//    this->registerCommand(SetEnemyDetector::NAME,static_cast<commandCallback>(&MotionExecutor::processSetEnemyDetector));

    //Subscribe to enemy detection notifications
    this->subscribe(EnemyDetectedNotification::NAME,static_cast<notificationCallback>(&MotionExecutor::processEnemyDetectedNotification));

    motionHandles[MotionCommand::MotionType::MOVE_STRAIGHT]=static_cast<motionCommandHandle>(&MotionExecutor::moveForward);
    motionHandles[MotionCommand::MotionType::MOVE_TO_POSITION]=static_cast<motionCommandHandle>(&MotionExecutor::moveToPosition);
    motionHandles[MotionCommand::MotionType::ROTATE_FOR]=static_cast<motionCommandHandle>(&MotionExecutor::rotateFor);
    motionHandles[MotionCommand::MotionType::ROTATE_TO]=static_cast<motionCommandHandle>(&MotionExecutor::rotateTo);
    motionHandles[MotionCommand::MotionType::MOVE_ARC]=static_cast<motionCommandHandle>(&MotionExecutor::moveArc);
    motionHandles[MotionCommand::MotionType::STOP]=static_cast<motionCommandHandle>(&MotionExecutor::stopMovement);
    motionHandles[MotionCommand::MotionType::SET_SPEED]=static_cast<motionCommandHandle>(&MotionExecutor::setSpeed);
    motionHandles[MotionCommand::MotionType::SET_POSITION]=static_cast<motionCommandHandle>(&MotionExecutor::setPosition);

    memset(detectedSensor,0,sizeof(detectedSensor));    //Set whole array to false
}

void MotionExecutor::processEnemyDetectedNotification(Notification* notification){
    EnemyDetectedNotification* ed=static_cast<EnemyDetectedNotification*>(notification);

    detectedSensor[ed->getType()]=ed->isDetected();

    std::stringstream ss;
    if (ed->getType()==EnemyDetectedNotification::Type::BRKON_BACK){
        ss<<"BACK BRKON @ "<<ed->getAngle()<<" detected: "<<ed->isDetected();
    }

    if (ed->getType()==EnemyDetectedNotification::Type::BRKON_FRONT){
        ss<<"FRONT BRKON @ "<<ed->getAngle()<<" detected: "<<ed->isDetected();
    }
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

void MotionExecutor::processSetEnemyDetector(Command* command){
    debug("Processing enemy detector");
    SetEnemyDetector* cmd=static_cast<SetEnemyDetector*>(command);
    switch (cmd->getType()) {
    case SetEnemyDetector::Type::DETECT_ALL:
        useEnemyDetector=true;
        checkField=true;
        break;
    case SetEnemyDetector::Type::NO_FIELD_CHECK:
        useEnemyDetector=true;
        checkField=false;
        break;
    case SetEnemyDetector::Type::DONT_CHECK:
        useEnemyDetector=false;
        checkField=false;
        break;
    default:
        break;
    }
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

        /* Proverim da li robot moze da ide tamo gde se uputio */
        if (useEnemyDetector){
            if (driver.getState()==MotionDriver::State::MOVING){
                if (detectedSensor[EnemyDetectedNotification::LEFT] || detectedSensor[EnemyDetectedNotification::RIGHT]){
                    if (driver.getDirection()==MotionDriver::MovingDirection::FORWARD && isInField(0,rSensor)){
                        if (currentMotionCommand!=NULL){
                            MotionCommandError* errorMsg=new MotionCommandError(MotionCommandError::ENEMY,currentMotionCommand->getSource());
                            errorMsg->setId(currentMotionCommand->getId());
                            sendResponse(errorMsg);
    //                        delete currentMotionCommand;
                            currentMotionCommand=NULL;
                        }
                        driver.stop();
                    }
                }
                if (detectedSensor[EnemyDetectedNotification::BACK]){
                    if (driver.getDirection()==MotionDriver::MovingDirection::BACKWARD && isInField(180,rSensor)){
                        if (currentMotionCommand!=NULL){
                            MotionCommandError* errorMsg=new MotionCommandError(MotionCommandError::ENEMY,currentMotionCommand->getSource());
                            errorMsg->setId(currentMotionCommand->getId());
                            sendResponse(errorMsg);
    //                        delete currentMotionCommand;
                            currentMotionCommand=NULL;
                        }
                        driver.stop();
                    }
                }
            }
        }

        /*Dobavim sledecu komandu*/
        MotionCommand* newCommand=getNextMotionCommand();
        if (newCommand!=NULL && currentMotionCommand!=NULL){
            debug("Newer command received, sending error to old");
            MotionCommandError* errorMsg=new MotionCommandError(MotionCommandError::OLD_COMMAND,currentMotionCommand->getSource());
            errorMsg->setId(currentMotionCommand->getId());
            sendResponse(errorMsg);
//            delete currentMotionCommand;
            currentMotionCommand=NULL;
        }



        //Process received command
        try{
            /*Sad bi trebalo tu komandu odraditi*/
            if (newCommand!=NULL){
                (this->*motionHandles[newCommand->getMotionType()])(newCommand);
            }
        }catch(...){
            error("***** Error in UART communication! ****");
            if (currentMotionCommand!=NULL){
                MotionCommandError* errorMsg=new MotionCommandError(MotionCommandError::UART,currentMotionCommand->getSource());
                errorMsg->setId(currentMotionCommand->getId());
                sendResponse(errorMsg);
//                delete currentMotionCommand;
                currentMotionCommand=NULL;
            }
            return;
        }

        //Update data from dirver
        try{
            driver.refreshData();
        }catch(...){
            error("***** Error in UART communication! ****");
            if (currentMotionCommand!=NULL){
                MotionCommandError* errorMsg=new MotionCommandError(MotionCommandError::UART,currentMotionCommand->getSource());
                errorMsg->setId(currentMotionCommand->getId());
                sendResponse(errorMsg);
//                delete currentMotionCommand;
                currentMotionCommand=NULL;
            }
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
            error("Robot stuck, canceling movement");
            MotionCommandError* errorMsg=new MotionCommandError(MotionCommandError::STUCK,currentMotionCommand->getSource());
            errorMsg->setId(currentMotionCommand->getId());
            sendResponse(errorMsg);
//            delete currentMotionCommand;
            currentMotionCommand=NULL;
            driver.stop();
        }

        //Send progress if there is a command for it
        if (lastState!=newState){
            if((currentMotionCommand!=NULL) ){
                GetMotionStateResponse* progress=new GetMotionStateResponse(currentMotionCommand->getSource(),
                         currentMotionCommand->getDestination(),newState,ResponseStatus::PROGRESS_UPDATE);
                progress->setId(currentMotionCommand->getId());
                sendResponse(progress);
            }

            int delta=newState.Position.euclidDist(lastNotificationState.Position);

            stateLock.lock();
            lastState=newState;
            stateLock.unlock();

            if (delta>MOTION_TRESHOLD){
                MotionNotification* motionNotification=new MotionNotification(newState);
                sendNotification(motionNotification);
                lastNotificationState=newState;
            }
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
    driver.rotateTo(command->getAbsoluteAngle());
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
    StopMovement* sm=(StopMovement*)_motionCommand;
    if (sm->isHardReset()){
        debug("Stopping movemen HARD");
        currentMotionCommand=_motionCommand;
        driver.stop();
    }else{
        debug("Stopping movemen SOFT");
        currentMotionCommand=_motionCommand;
        driver.softStop();
    }
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

bool MotionExecutor::isInField(int angle, int r){
    if (!checkField) return true;
    Point2D robotPosition=driver.getPosition();
    int rotation= driver.getOrientation()+angle;
    Point2D enemyPosition(robotPosition.getX()+r*cos(rotation),robotPosition.getY()+r*sin(rotation));
    if (maxX - abs(enemyPosition.getX())<0){
        return false;
    }
    if ((enemyPosition.getY()<0) ||(enemyPosition.getY()>maxY)){
        return false;
    }

    return true;
}

}
