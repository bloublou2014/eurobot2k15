#include "MotionExecutor.h"

#include <iostream>

#define MOTION_TRESHOLD 10
#define SLOW_DISTANCE 100
#define SLOW_SPEED 50

using namespace std;

namespace motion {

string MotionExecutor::NAME="MotionExecutor";
string MotionExecutor::CONFIG_FILENAME="MotionConfig.xml";

//Obavezno koristiti static_cast
void MotionExecutor::init(){
    using namespace boost::property_tree;
    ptree pt;

    int rBrkon;
    int rSensor;
    read_xml(CONFIG_FILENAME, pt);
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("motionConfig"))
    {
        rBrkon=v.second.get<int>("rBrkon");
        rSensor=v.second.get<int>("rSensor");
        maxX=v.second.get<int>("maxX");
        maxY=v.second.get<int>("maxY");
        minX=v.second.get<int>("minX");
        minY=v.second.get<int>("minY");
        enemyDistance=v.second.get<int>("enemyDistance");
        triangleSide=v.second.get<int>("triangleSide");
    }

    this->registerCommand(MotionCommand::NAME,static_cast<commandCallback>(&MotionExecutor::processMotionCommand));
    this->registerCommand(GetMotionState::NAME,static_cast<commandCallback>(&MotionExecutor::processGetMotionState));
//    this->registerCommand(SetEnemyDetector::NAME,static_cast<commandCallback>(&MotionExecutor::processSetEnemyDetector));
    this->registerCommand(AddStaticObject::NAME,static_cast<commandCallback>(&MotionExecutor::processAddObstacle));

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

    //Initialize sensor distances
    enemySensors[EnemyDetectedNotification::Type::BRKON_FRONT].Distance=rBrkon;
    enemySensors[EnemyDetectedNotification::Type::BRKON_BACK].Distance=rBrkon;
    enemySensors[EnemyDetectedNotification::Type::LEFT].Distance=rSensor;
    enemySensors[EnemyDetectedNotification::Type::RIGHT].Distance=rSensor;
    enemySensors[EnemyDetectedNotification::Type::BACK].Distance=rSensor;

    enemySensors[EnemyDetectedNotification::Type::BRKON_FRONT].Direction=MotionDriver::MovingDirection::FORWARD;
    enemySensors[EnemyDetectedNotification::Type::BRKON_BACK].Direction=MotionDriver::MovingDirection::BACKWARD;
    enemySensors[EnemyDetectedNotification::Type::LEFT].Direction=MotionDriver::MovingDirection::FORWARD;
    enemySensors[EnemyDetectedNotification::Type::RIGHT].Direction=MotionDriver::MovingDirection::FORWARD;
    enemySensors[EnemyDetectedNotification::Type::BACK].Direction=MotionDriver::MovingDirection::BACKWARD;
    enemySensorCount=5;

    pathFinder=new PathFinding(maxX, minX, maxY, minY);
}

void MotionExecutor::processEnemyDetectedNotification(Notification* notification){
    EnemyDetectedNotification* ed=static_cast<EnemyDetectedNotification*>(notification);
    pfLock.lock();
    enemySensors[ed->getType()].Detected=ed->isDetected();
    int angle=ed->getAngle();
    enemySensors[ed->getType()].Angle=angle;
    //Izbacim iz pf ako je nesto vec bilo ubaceno
    if(enemySensors[ed->getType()].obstacleId!=-1){
        pathFinder->removeObstacle(enemySensors[ed->getType()].obstacleId);
    }
    if (ed->isDetected()){
        stateLock.lock();
        Point2D enemyPosition=lastState.Position;
        stateLock.unlock();
        enemyPosition.setX(enemyPosition.getX()+enemyDistance*cos(angle));
        enemyPosition.setY(enemyPosition.getY()+enemyDistance*sin(angle));
        enemySensors[ed->getType()].obstacleId=dodajSestougao(enemyPosition.getX(),enemyPosition.getY(),triangleSide);
    }
    pfLock.unlock();

    std::stringstream ss;

    ss<<"Enemy detected type: "<<ed->getType()<<" detected: "<<ed->isDetected()<<" ";
//    if (ed->getType()==EnemyDetectedNotification::Type::BRKON_BACK){
//        ss<<"BACK BRKON @ "<<ed->getAngle();
//    }

//    if (ed->getType()==EnemyDetectedNotification::Type::BRKON_FRONT){
//        ss<<"FRONT BRKON @ "<<ed->getAngle();
//    }

    debug(ss.str());
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

const int MotionInstruction::MaxRetryCount=500;
bool MotionExecutor::isEnemyDetected(MotionState& ms){
    if (ms.State==MotionDriver::State::ROTATING) return false;  //Fix zbog malog
    for(int i=0;i<enemySensorCount;++i){
        //If we are moving in direction of detected obsticle
        if (enemySensors[i].Direction==ms.Direction && enemySensors[i].Detected){
            if (isInField(enemySensors[i].Angle,enemySensors[i].Distance)){
                //than enemy is detected in moving direction
                std::stringstream ss;
                ss<<"Enemy detected: "<<i;
//                debug(ss.str());
                return true;
            }
        }
    }
    return false;
}

bool MotionExecutor::shouldUseSlow(int distance){
    return distance<SLOW_DISTANCE;  //if it's smaller than slow hould be used
}

bool MotionExecutor::shouldUseSlow(Point2D second){
    return lastState.Position.euclidDist(second)<SLOW_DISTANCE;
}

void MotionExecutor::main(){
    shouldStop=false;
    debug("Started main thread execution");

    bool waitOnEnemyCountCheck=true;
#ifdef MALI_ROBOT
    waitOnEnemyCountCheck=false;
#endif

    MotionState newState;
    while (true){
        if (shouldStop){
            driver.stop();
            break;
        }

        /* Proverim da li robot moze da ide tamo gde se uputio */
        if (currentMotionInstruction.isSet()){
            if (isEnemyDetected(newState)){
                if (currentMotionInstruction.isSuspended()){
                    if (!currentMotionInstruction.canRetry(waitOnEnemyCountCheck)){  //If we can't wait any more
                        bool giveUp=true;
                        if (currentMotionInstruction.shouldUsePF()){
                            debug("Calculating alternative route");
                            pfLock.lock();
                            std::deque<geometry::Point2D> ret;
                            Point2D destination=currentMotionInstruction.getDestination().Position;
                            giveUp=!pathFinder->search(newState.Position,destination,
                                                       currentMotionInstruction.getPfPositions());
                            pfLock.unlock();
                            debug("Finished cacluating alternative route");
                            currentMotionInstruction.resume(driver);
                        }
                        if (giveUp){
                            debug("****** Sending error, timeout *******");
                            sendResponse(currentMotionInstruction.getErrorMessage(MotionCommandError::ENEMY));
                            currentMotionInstruction.reset(driver);
                            driver.stop();
                        }
                    }
                }else{
                    debug("******* Pausing execution *******");
                    currentMotionInstruction.pause(driver);
                }
            }else if (currentMotionInstruction.isSuspended()){
                debug("Resuming motion");
                if (shouldUseSlow(currentMotionInstruction.getDestination().Position)){
                    currentMotionInstruction.saveSpeed(driver.getSpeed());
                    driver.setSpeed(SLOW_SPEED);
                }
                currentMotionInstruction.resume(driver);
            }
        }

        /*Dobavim sledecu komandu*/
        MotionCommand* newCommand=getNextMotionCommand();
        if (newCommand!=NULL && currentMotionInstruction.isSet()){
            debug("Newer command received, sending error to old");
            sendResponse(currentMotionInstruction.getErrorMessage(MotionCommandError::OLD_COMMAND));
            currentMotionInstruction.reset(driver);
        }

        //Process received command
        try{
            /*Sad bi trebalo tu komandu odraditi*/
            if (newCommand!=NULL){
                (this->*motionHandles[newCommand->getMotionType()])(newCommand);
            }
        }catch(...){
            error("***** Error in UART communication! ****");
            if (currentMotionInstruction.isSet()){
                sendResponse(currentMotionInstruction.getErrorMessage(MotionCommandError::UART));
                currentMotionInstruction.reset(driver);
            }
            return;
        }

        //Update data from dirver
        try{
            driver.refreshData();
        }catch(...){
            error("***** Error in UART communication! ****");
            if (currentMotionInstruction.isSet()){
                sendResponse(currentMotionInstruction.getErrorMessage(MotionCommandError::UART));
                currentMotionInstruction.reset(driver);
            }
            return;
        }
        newState.Direction=driver.getDirection();
        newState.Orientation=driver.getOrientation();
        newState.Position=driver.getPosition();
        newState.Speed=driver.getSpeed();
        newState.State=driver.getState();

        //If robot finished movement, report it back
        if (newState.State==MotionDriver::State::IDLE && currentMotionInstruction.isSet()
                && !currentMotionInstruction.isSuspended()){

            if (!currentMotionInstruction.hasMorePoints()){
                debug("Robot finished executing command");
                sendResponseFromCommand(currentMotionInstruction.getCommand());
                currentMotionInstruction.reset(driver);
            }else{
                currentMotionInstruction.moveToNextPoint(driver);
            }
        }

        //If driver throws an error, report it
        if ((newState.State==MotionDriver::State::ERROR || newState.State==MotionDriver::State::STUCK)
                && currentMotionInstruction.isSet()){
            error("Robot stuck, canceling movement");
            sendResponse(currentMotionInstruction.getErrorMessage(MotionCommandError::STUCK));
            driver.stop();
            currentMotionInstruction.reset(driver);
        }

        //Send progress if there is a command for it
        if (lastState!=newState){
            if((currentMotionInstruction.isSet()) ){
                MotionCommand* motionCommand=currentMotionInstruction.getCommand();
                GetMotionStateResponse* progress=new GetMotionStateResponse(motionCommand->getSource(),
                         motionCommand->getDestination(), newState, ResponseStatus::PROGRESS_UPDATE);
                progress->setId(motionCommand->getId());
                sendResponse(progress);
            }

            stateLock.lock();
            lastState=newState;
            stateLock.unlock();

            int delta=newState.Position.euclidDist(lastNotificationState.Position);
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
    MotionState destinationState=lastState;
    destinationState.Position=command->getPosition();
    destinationState.Direction=command->getDirection();
    currentMotionInstruction.Set(_motionCommand,destinationState, command->shouldUsePathFinder());

    if (command->shouldUsePathFinder()){
        pfLock.lock();
        pathFinder->search(lastState.Position,destinationState.Position,currentMotionInstruction.getPfPositions());
        pfLock.unlock();
        currentMotionInstruction.moveToNextPoint(driver);
    }else{
        if (shouldUseSlow(destinationState.Position)){
            currentMotionInstruction.saveSpeed(driver.getSpeed());
            driver.setSpeed(SLOW_SPEED);
        }
        driver.moveToPosition(destinationState.Position,destinationState.Direction);
    }

    driver.moveToPosition(destinationState.Position,destinationState.Direction);
}

void MotionExecutor::moveForward(MotionCommand* _motionCommand){
    MoveForward* command=(MoveForward*)_motionCommand;
    debug("Moving forward");
    MotionState destinationState=lastState;
    int distance=command->getDistance();
    destinationState.Position.setX(destinationState.Position.getX()+distance*cos(destinationState.Orientation));
    destinationState.Position.setY(destinationState.Position.getY()+distance*sin(destinationState.Orientation));
    if (distance<0)
        destinationState.Direction=MotionDriver::MovingDirection::BACKWARD;
    else
        destinationState.Direction=MotionDriver::MovingDirection::FORWARD;
    currentMotionInstruction.Set(_motionCommand, destinationState);

    if (shouldUseSlow(distance)){
        currentMotionInstruction.saveSpeed(driver.getSpeed());
        driver.setSpeed(SLOW_SPEED);
    }

    driver.moveStraight(distance);
}

void MotionExecutor::rotateFor(MotionCommand* _motionCommand){
    RotateFor* command=(RotateFor*)_motionCommand;
    debug("Rotating for");
    MotionState destinationState=lastState;
    destinationState.Orientation+=command->getRelativeAngle();
    currentMotionInstruction.Set(_motionCommand,destinationState);
    driver.rotateFor(destinationState.Orientation);
}

void MotionExecutor::rotateTo(MotionCommand* _motionCommand){
    RotateTo* command=(RotateTo*)_motionCommand;
    debug("Rotating to");
    MotionState destinationState=lastState;
    destinationState.Orientation=command->getAbsoluteAngle();
    currentMotionInstruction.Set(_motionCommand, destinationState);
    driver.rotateTo(destinationState.Orientation);
}

void MotionExecutor::moveArc(MotionCommand* _motionCommand){
    MoveArc* command=(MoveArc*)_motionCommand;
    debug("Moving arc");
    currentMotionInstruction.Set(_motionCommand, lastState);
    driver.moveArc(command->getCenter(),command->getAngle(),command->getDirection());
}

void MotionExecutor::setSpeed(MotionCommand* _motionCommand){
    debug("Setting speed");
    SetSpeedMotion* command=(SetSpeedMotion*)_motionCommand;
    currentMotionInstruction.Set(_motionCommand, lastState);
    std::stringstream ss;
    ss<<"Set speed @ "<<command->getSpeed();
    debug(ss.str());
    driver.setSpeed(command->getSpeed());
}

void MotionExecutor::setPosition(MotionCommand* _motionCommand){
    debug("Setting position");
    SetPosition* command=(SetPosition*)_motionCommand;
    currentMotionInstruction.Set(_motionCommand, lastState);
    std::stringstream ss;
    debug(ss.str());
    driver.setPositionAndOrientation(command->getPoint(),command->getOrientation());
}


void MotionExecutor::stopMovement(MotionCommand* _motionCommand){
    StopMovement* sm=(StopMovement*)_motionCommand;
    if (sm->isHardReset()){
        debug("Stopping movemen HARD");
        currentMotionInstruction.Set(_motionCommand, lastState);
        driver.stop();
    }else{
        debug("Stopping movemen SOFT");
        currentMotionInstruction.Set(_motionCommand, lastState);
        driver.softStop();
    }
}

bool MotionExecutor::isInField(int angle, int r){
    if (!checkField) return true;
    Point2D robotPosition=driver.getPosition();
    int rotation= driver.getOrientation()+angle;
    Point2D enemyPosition(robotPosition.getX()+r*cos(rotation),robotPosition.getY()+r*sin(rotation));
    if ((enemyPosition.getX()<minX) || (enemyPosition.getX()>maxX)){
        return false;
    }
    if ((enemyPosition.getY()<minY) || (enemyPosition.getY()>maxY)){
        return false;
    }
//    std::stringstream ss;
    //ss<<"Enemy detected in field: "<<robotPosition.getX()<<", "<<robotPosition.getY()<<" enemy: "<<enemyPosition.getX()<<", "<<enemyPosition.getY();
    //debug(ss.str());
    return true;
}

int MotionExecutor::dodajSestougao(int krugX, int krugY, int triangleSide){
    int triangleHeight = (sqrt(3)/2)*triangleSide;
    std::vector<geometry::Point2D> obsticlePoints;

    obsticlePoints.push_back(geometry::Point2D(krugX + triangleSide,     krugY));
    obsticlePoints.push_back(geometry::Point2D(krugX + (triangleSide/2), krugY - triangleHeight));
    obsticlePoints.push_back(geometry::Point2D(krugX - (triangleSide/2), krugY - triangleHeight));
    obsticlePoints.push_back(geometry::Point2D(krugX - triangleSide,     krugY));
    obsticlePoints.push_back(geometry::Point2D(krugX - (triangleSide/2), krugY + triangleHeight));
    obsticlePoints.push_back(geometry::Point2D(krugX + (triangleSide/2), krugY + triangleHeight));
    return pathFinder->addObstacle(obsticlePoints);
}

}
