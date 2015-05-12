#ifndef _MOTIONEXECUTOR_H
#define _MOTIONEXECUTOR_H

#include <deque>
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
#include "utils/pathFinding/PathFinding.h"
#include "executors/msg/EnemyDetectorCommand.h"

using namespace robot;
using namespace path_finding;
using boost::mutex;
using std::queue;
using std::deque;
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
        usePathFinder=false;
        pfPositions.clear();
    }

    void Set(MotionCommand* _motionCommand, MotionState& _destination, bool _usePf=false){
        destination=_destination;
        suspended=false;
        command=_motionCommand;
        slowSpeed=false;
        previousSpeed=0;
        retryCount=0;
        usePathFinder=_usePf;
        pfPositions.clear();
    }

    void pause(MotionDriver& driver){
        suspended=true;
        driver.stop();
    }

    void resume(MotionDriver& driver){
        suspended=false;
        retryCount=0;
        if (pfPositions.size()>0){
            driver.moveToPosition(pfPositions.front(),destination.Direction);
        }else{
            driver.moveToPosition(destination.Position,destination.Direction);
        }
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

    bool shouldUsePF() const{
        return usePathFinder;
    }

    std::deque<geometry::Point2D>& getPfPositions(){
        return pfPositions;
    }

    void moveToNextPoint(MotionDriver& driver){
        pfPositions.pop_front();
        resume(driver);
    }

    bool hasMorePoints(){
        if (usePathFinder)
            return pfPositions.size()>0;
        return false;
    }

private:
    MotionDriver::MovingDirection direction;
    MotionCommand* command;
    MotionState destination;
    int retryCount;
    bool suspended;
    bool usePathFinder;
    bool slowSpeed;
    int previousSpeed;
    std::deque<geometry::Point2D> pfPositions;
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
    void processAddObstacle(Command* command);

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

    bool isEnemyDetected(MotionState& ms, bool isSuspended);

    int rSensor;
    bool useEnemyDetector;
    struct Enemy{
        Enemy(int _angle=0):Angle(_angle),Detected(false){}
        int Angle;
        bool Detected;
    };
    Enemy detectedEnemies[2];

    /* Ignoring outer field obsticles*/
    int maxX;
    int maxY;
    int minX;
    int minY;
    bool checkField;
    bool isInField(int angle, int r);

    /* for slowing down motion on small distances */
    bool shouldUseSlow(int distance, int originalSpeed);
    bool shouldUseSlow(geometry::Point2D _position, int originalSpeed);

    mutex pfLock;
    PathFinding* pathFinder;
    vector<Point2D> enemyDimensions;
    int dodajSestougao(int krugX, int krugY, int triangleSide);
    int enemyDistance;
    int triangleSide;
};

}

#endif //_MOTIONEXECUTOR_H
