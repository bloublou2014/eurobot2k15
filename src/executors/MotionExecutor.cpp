#include "MotionExecutor.h"

#include <iostream>

#define MOTION_TRESHOLD 10
#define SLOW_DISTANCE 100
#define SLOW_SPEED 50
static double PI=3.141592653584;

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
        rSensor=v.second.get<int>("rSensor");
        maxX=v.second.get<int>("maxX");
        maxY=v.second.get<int>("maxY");
        minX=v.second.get<int>("minX");
        minY=v.second.get<int>("minY");
        margin=v.second.get<int>("margin");

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
    pathFinder=new PathFinding(maxX, minX, maxY, minY);
}

void MotionExecutor::processEnemyDetectedNotification(Notification* notification){
    debug("Enemy detected notification received");
    EnemyDetectedNotification* ed=static_cast<EnemyDetectedNotification*>(notification);
    EnemyDetectedNotification::Type detectedType=ed->getType();
    std::stringstream ss;
    int angle=ed->getAngle();

    pfLock.lock();    //Path finding logic
    if (!ed->isDetected()){
        detectedEnemies[detectedType].Detected=false;
        detectedEnemies[detectedType].EnemyLeft=true;
        ss<<"Enemy NOT detected: "<<ed->getType()<<" detected angle: "<<ed->getAngle();

        if(detectedEnemies[detectedType].Id!=-1){
            pathFinder->removeObstacle(detectedEnemies[detectedType].Id);
            debug("Removed enemy from PF");
            detectedEnemies[detectedType].Id=-1;
        }
    }else{
        detectedEnemies[detectedType].Detected=true;
        ss<<"Enemy detected type: "<<ed->getType()<<" detected angle: "<<ed->getAngle();

        stateLock.lock();
        Point2D enemyPosition=lastState.Position;
        angle+=lastState.Orientation;
        stateLock.unlock();
        enemyPosition.setX(enemyPosition.getX()+enemyDistance*cos(angle*PI/180.0));
        enemyPosition.setY(enemyPosition.getY()+enemyDistance*sin(angle*PI/180.0));

        stringstream ss;
        ss<<"Calculated enemy position: "<<enemyPosition;
        debug(ss.str());

        if (detectedEnemies[detectedType].Id!=-1){
            pathFinder->removeObstacle(detectedEnemies[detectedType].Id);
        }

        detectedEnemies[detectedType].Id=dodajSestougao(enemyPosition.getX(),enemyPosition.getY(),triangleSide);

        if (!detectedEnemies[detectedType].EnemyLeft && enemyPosition.euclidDist(detectedEnemies[detectedType].Position)<=200){
            debug("Enemy is in fact not detected!");
            detectedEnemies[detectedType].Detected=false;
        }
        detectedEnemies[detectedType].EnemyLeft=false;
        detectedEnemies[detectedType].Position=enemyPosition;

        debug("Added new enemy in PF");
    }
    pfLock.unlock();

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

void MotionExecutor::processAddObstacle(Command* command){
    pfLock.lock();
    AddStaticObject* cmd=(AddStaticObject*)command;
    pathFinder->addObstacle(cmd->getEdges());
    pfLock.unlock();

    sendResponseFromCommand(command);
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

const int MotionInstruction::MaxRetryCount=80;
bool MotionExecutor::isEnemyDetected(MotionState& ms, bool isSuspended){
    if (ms.State!=MotionDriver::State::MOVING && !isSuspended) return false;  //We won't use enemy detection when not moving
    EnemyDetectedNotification::Type detectedType;
    if (ms.Direction==MotionDriver::MovingDirection::FORWARD){
        detectedType=EnemyDetectedNotification::Type::FRONT;
    }else{
        detectedType=EnemyDetectedNotification::Type::BACK;
    }
    if (!detectedEnemies[detectedType].Detected)
        return false;  //We didn't find anything on moving side
    if (isInField(detectedEnemies[detectedType].Position)){
        //than enemy is detected in moving direction
        return true;
    }
    return false;
}

bool MotionExecutor::shouldUseSlow(int distance,int originalSpeed){
    if (originalSpeed<=SLOW_SPEED) return false;
    return abs(distance)<SLOW_DISTANCE;  //if it's smaller than slow hould be used
}

bool MotionExecutor::shouldUseSlow(Point2D second, int originalSpeed){
    if (originalSpeed<=SLOW_SPEED) return false;
    return abs(lastState.Position.euclidDist(second))<SLOW_DISTANCE;
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

        /* Proverim da li robot moze da ide tamo gde se uputio */
        if (currentMotionInstruction.isSet()){
            if (isEnemyDetected(newState, currentMotionInstruction.isSuspended())){
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

                            cout<<"**Number of obstacles: "<<pathFinder->getAllObstacles().size()<<endl;

                            pfLock.unlock();
                            debug("Finished cacluating alternative route");
                            if (!giveUp){
                                debug("Moving to next position returned from PF");
                                currentMotionInstruction.moveToNextPoint(driver);
                                // if we found path than remove detected enemy
                                debug("Deleting detected enemy");
                                EnemyDetectedNotification::Type detectedType;
                                if (newState.Direction==MotionDriver::MovingDirection::FORWARD){
                                    detectedType=EnemyDetectedNotification::Type::FRONT;
                                }else{
                                    detectedType=EnemyDetectedNotification::Type::BACK;
                                }
                                detectedEnemies[detectedType].Detected=false;
                            }
                        }
                        if (giveUp){
                            debug("****** Sending error, timeout, enemy detected *******");
                            sendResponse(currentMotionInstruction.getErrorMessage(MotionCommandError::ENEMY));
                            currentMotionInstruction.reset(driver);
                            driver.stop();
                        }
                    }
                }else{
                    debug("******* Pausing motion, waiting for enemy to leave*******");
                    currentMotionInstruction.pause(driver);
                }
            }else if (currentMotionInstruction.isSuspended()){
                debug("Resuming motion");
                if (shouldUseSlow(currentMotionInstruction.getDestination().Position,driver.getSpeed())){
                    debug("Slowing down, small distance!");
                    currentMotionInstruction.saveSpeed(driver.getSpeed());
                    driver.setSpeed(SLOW_SPEED);
                }
                debug("Resuming movement");
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
                debug("Moving to next point");
                currentMotionInstruction.moveToNextPoint(driver);
            }
        }

        //If driver throws an error, report it
        if ((newState.State==MotionDriver::State::ERROR || newState.State==MotionDriver::State::STUCK)
                && currentMotionInstruction.isSet()){
            error("Robot stuck, canceling movement");
            sendResponse(currentMotionInstruction.getErrorMessage(MotionCommandError::STUCK));
            //driver.stop();
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
    debug(command->getSource()+" Received: Moving to position");
    MotionState destinationState=lastState;
    destinationState.Position=command->getPosition();
    destinationState.Direction=command->getDirection();
    currentMotionInstruction.Set(_motionCommand,destinationState, command->shouldUsePathFinder());

    if (command->shouldUsePathFinder()){
        bool pathFound=false;

        pfLock.lock();
        pathFound=pathFinder->search(lastState.Position,destinationState.Position,currentMotionInstruction.getPfPositions());
        pfLock.unlock();

        for (auto it=currentMotionInstruction.getPfPositions().begin();it!=currentMotionInstruction.getPfPositions().end();++it){
            std::stringstream ss;
            ss<<"Path finder point: "<<*it;
            debug(ss.str());
        }

        if (pathFound){
            debug("Path found!");
            currentMotionInstruction.moveToNextPoint(driver);
        }else{
            debug("Path not found!");
            sendResponse(currentMotionInstruction.getErrorMessage(MotionCommandError::ENEMY));
            currentMotionInstruction.reset(driver);
        }
    }else{
        if (shouldUseSlow(destinationState.Position,driver.getSpeed())){
            debug("Slowing down2");
            currentMotionInstruction.saveSpeed(driver.getSpeed());
            driver.setSpeed(SLOW_SPEED);
        }
        driver.moveToPosition(destinationState.Position,destinationState.Direction);
    }
}

void MotionExecutor::moveForward(MotionCommand* _motionCommand){
    MoveForward* command=(MoveForward*)_motionCommand;
    debug(command->getSource()+" Received: Move forward");
    MotionState destinationState=lastState;
    int distance=command->getDistance();
    destinationState.Position.setX(destinationState.Position.getX()+distance*cos(destinationState.Orientation*PI/180.0));
    destinationState.Position.setY(destinationState.Position.getY()+distance*sin(destinationState.Orientation*PI/180.0));
    if (distance<0)
        destinationState.Direction=MotionDriver::MovingDirection::BACKWARD;
    else
        destinationState.Direction=MotionDriver::MovingDirection::FORWARD;
    currentMotionInstruction.Set(_motionCommand, destinationState);

    if (shouldUseSlow(distance,driver.getSpeed())){
        debug("Slowing down3");
        currentMotionInstruction.saveSpeed(driver.getSpeed());
        driver.setSpeed(SLOW_SPEED);
    }

    driver.moveStraight(distance);
}

void MotionExecutor::rotateFor(MotionCommand* _motionCommand){
    RotateFor* command=(RotateFor*)_motionCommand;
    debug(command->getSource()+" Received: Rotate for");
    MotionState destinationState=lastState;
    destinationState.Orientation+=command->getRelativeAngle();
    currentMotionInstruction.Set(_motionCommand,destinationState);
    driver.rotateFor(destinationState.Orientation);
}

void MotionExecutor::rotateTo(MotionCommand* _motionCommand){
    RotateTo* command=(RotateTo*)_motionCommand;
    debug(command->getSource()+" Received: Rotate to");
    MotionState destinationState=lastState;
    destinationState.Orientation=command->getAbsoluteAngle();
    currentMotionInstruction.Set(_motionCommand, destinationState);
    driver.rotateTo(destinationState.Orientation);
}

void MotionExecutor::moveArc(MotionCommand* _motionCommand){
    MoveArc* command=(MoveArc*)_motionCommand;
    debug(command->getSource()+" Received: Move arc");
    currentMotionInstruction.Set(_motionCommand, lastState);
    driver.moveArc(command->getCenter(),command->getAngle(),command->getDirection());
}

void MotionExecutor::setSpeed(MotionCommand* _motionCommand){
    SetSpeedMotion* command=(SetSpeedMotion*)_motionCommand;
    debug(command->getSource()+" Received: Set speed");
    currentMotionInstruction.Set(_motionCommand, lastState);
    std::stringstream ss;
    ss<<"Set speed @ "<<command->getSpeed();
    debug(ss.str());
    driver.setSpeed(command->getSpeed());
}

void MotionExecutor::setPosition(MotionCommand* _motionCommand){
    SetPosition* command=(SetPosition*)_motionCommand;
    debug(command->getSource()+" Received: Set position");
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

static double PI=3.141592653584;
bool MotionExecutor::isInField(Point2D& enemyPosition){
    static int xMinC=minX+margin;
    static int xMaxC=maxX-margin;
    static int yMinC=minY+margin;
    static int yMaxC=maxY-margin;

//    if (!checkField) return true;
//    Point2D robotPosition=driver.getPosition();
//    int rotation=driver.getOrientation()+angle;

//    Point2D enemyPosition(robotPosition.getX()+r*cos(rotation*PI/180.0),robotPosition.getY()+r*sin(rotation*PI/180.0));
    if ((enemyPosition.getX()<xMinC) || (enemyPosition.getX()>xMaxC)){
        return false;
    }
    if ((enemyPosition.getY()<yMinC) || (enemyPosition.getY()>yMaxC)){
        return false;
    }
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

    for (auto it=obsticlePoints.begin();it!=obsticlePoints.end();++it){
        stringstream ss;
        ss<<"Point: "<<*it;
        debug(ss.str());
    }

    return pathFinder->addObstacle(obsticlePoints);
}

}
