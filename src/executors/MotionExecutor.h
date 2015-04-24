#ifndef _MOTIONEXECUTOR_H
#define _MOTIONEXECUTOR_H

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <queue>
#include <map>
#include <cmath>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/atomic.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "AbstractExecutor.h"
#include "executors/msg/MotionCommand.h"
#include "executors/msg/GetMotionState.h"
#include "drivers/motion/MotionDriver.h"
#include "executors/msg/MotionNotification.h"
#include "executors/msg/EnemyDetectedNotification.h"

using namespace robot;
using boost::mutex;
using std::queue;
using std::pair;
using std::max_element;

using executor::EnemyDetectedNotification;

namespace motion{

class MotionInstruction{
public:
    static const int MaxRetryCount;

    MotionInstruction():command(NULL),retryCount(0),suspended(false){}

    void reset(MotionDriver& driver){
        if (slowSpeed)
            driver.setSpeed(previousSpeed);
        slowSpeed=false;
        previousSpeed=0;
        suspended=false;
        retryCount=0;
        delete command;
        command=NULL;
    }

    void Set(MotionCommand* _motionCommand, MotionState& _destination){
        destination=_destination;
        suspended=false;
        command=_motionCommand;
        slowSpeed=false;
        previousSpeed=0;
        retryCount=0;
    }

    void pause(MotionDriver& driver){
        suspended=true;
        driver.stop();
    }

    void resume(MotionDriver& driver){
        suspended=false;
        retryCount=0;
        driver.moveToPosition(destination.Position,destination.Direction);
    }

    inline MotionCommand* getCommand(){
        return command;
    }

    inline bool isSuspended(){
        return suspended;
    }

    inline bool isSet(){
        return command!=NULL;
    }

    inline bool canRetry(bool increment=true){
        if (increment)
            return ++retryCount<MaxRetryCount;
        else
            return true;
    }

    MotionCommandError* getErrorMessage(MotionCommandError::ErrorType errorType){
        MotionCommandError* errorMsg=new MotionCommandError(errorType,command->getSource());
        errorMsg->setId(command->getId());
        return errorMsg;
    }

    inline MotionDriver::MovingDirection getDirection(){
        return direction;
    }

    void saveSpeed(int speed){
        slowSpeed=true;
        previousSpeed=speed;
    }

    MotionState getDestination(){
        return destination;
    }

private:
    MotionDriver::MovingDirection direction;
    MotionCommand* command;
    MotionState destination;
    int retryCount;
    bool suspended;

    bool slowSpeed;
    int previousSpeed;
};

class MotionExecutor: public AbstractExecutor{
public:
    static string CONFIG_FILENAME;
    static string NAME;
    MotionExecutor():AbstractExecutor(NAME),
        useEnemyDetector(true),checkField(true){}

    void init();
    void stop();

    void processMotionCommand(Command* command);
    void processGetMotionState(Command* command);
    void processSetEnemyDetector(Command* command);

    void processEnemyDetectedNotification(Notification* notification);
protected:
    void main();
private:
    mutex commandQueueLock;
    queue<Command*> commandsToProcess;
    MotionCommand* getNextMotionCommand();

    bool shouldStop;
    mutex stateLock;
    MotionState lastState;
    MotionState lastNotificationState;

    MotionInstruction currentMotionInstruction;

    MotionDriver driver;
    typedef void (MotionExecutor::*motionCommandHandle)(MotionCommand* _motionCommand);
    typedef map<MotionCommand::MotionType,motionCommandHandle> MotionCommandsHandleMap;
    MotionCommandsHandleMap motionHandles;

    void moveToPosition(MotionCommand* _motionCommand);
    void moveForward(MotionCommand* _motionCommand);
    void rotateFor(MotionCommand* _motionCommand);
    void rotateTo(MotionCommand* _motionCommand);
    void moveArc(MotionCommand* _motionCommand);
    void stopMovement(MotionCommand* _motionCommand);
    void setSpeed(MotionCommand* _motionCommand);
    void setPosition(MotionCommand* _motionCommand);

    /*Used for enemy detector and path finder*/
    static int MaxRetryCount;

    bool isEnemyDetected(MotionState& ms);
    //void handleEnemyDetection();
    bool useEnemyDetector;
    struct Enemy{
        Enemy():Detected(false){}
        bool Detected;
        int Angle;
        MotionDriver::MovingDirection Direction;
        int Distance;
    };
    Enemy enemySensors[10];
    int enemySensorCount;

    /* Ignoring outer field obsticles*/
    int maxX;
    int maxY;
    bool checkField;
    bool isInField(int angle, int r);

    bool shouldUseSlow(int distance);
    bool shouldUseSlow(geometry::Point2D _position);
};

}

#endif //_MOTIONEXECUTOR_H
