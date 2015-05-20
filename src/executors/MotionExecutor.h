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
#include "utils/geometry/GeometryUtil.h"

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
    static const int MaxPfAttempts;

    MotionInstruction():command(NULL),retryCount(0),suspended(false),stuckCount(0){}

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
        avoidAttempts=0;
        stuckCount=0;
    }

    void Set(MotionCommand* _motionCommand, MotionState& _destination, bool _usePf=false){
        destination=_destination;
        suspended=false;
        command=_motionCommand;
        slowSpeed=false;
        previousSpeed=0;
        retryCount=0;
        avoidAttempts=0;
        stuckCount=0;
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
//            driver.moveToPosition(pfPositions.front(),destination.Direction);
            MoveToAutoPosition(pfPositions.front(),driver);
        }else{
//            driver.moveToPosition(destination.Position,destination.Direction);
            MoveToAutoPosition(destination.Position,driver);
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


    bool canUsePf(){
        avoidAttempts++;
        if (avoidAttempts==MaxPfAttempts){
            return false;
        }
        return true;
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
        cout<<"**Move to next position: "<<pfPositions.front().getX()<<" Y: "<<pfPositions.front().getY()<<endl;

        resume(driver);
        pfPositions.pop_front();
    }

    bool hasMorePoints(){
        if (usePathFinder)
            return pfPositions.size()>0;
        return false;
    }

    void MoveToAutoPosition(const Point2D &position,  MotionDriver &driver)
    {
        MotionDriver::MovingDirection dir;
        {
            driver.refreshData();
            const geometry::Point2D currentPosition = driver.getPosition();
            const int currentOrientation = driver.getOrientation();

            const int angle = currentPosition.relativeAngleTo(position);

            int deltaAngle = angle - currentOrientation;

            deltaAngle = geometry::GeometryUtil::normalizeAngle(deltaAngle);

            if((deltaAngle < 90) && (deltaAngle > -90)){
                dir = MotionDriver::FORWARD;
            }
            else{
                dir = MotionDriver::BACKWARD;
            }
        }
        driver.moveToPosition(position, dir);
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
    int avoidAttempts;
    int stuckCount;
    std::deque<geometry::Point2D> pfPositions;
};

class MotionExecutor: public AbstractExecutor{
public:
    static string CONFIG_FILENAME;
    static string NAME;
    MotionExecutor():AbstractExecutor(NAME),
        useEnemyDetector(true),checkField(true), matchStarted(false){}

    void init();
    void stop();

    void processMotionCommand(Command* command);
    void processGetMotionState(Command* command);
    void processSetEnemyDetector(Command* command);
    void processAddObstacle(Command* command);

    void processEnemyDetectedNotification(Notification* notification);
    void startMatch();
protected:
    void main();
private:
    bool matchStarted;

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
        Enemy():Detected(false),Id(-1){}
        Point2D Position;   //Pozicija koja se koristi za path finder
        Point2D CentralPosition;    //Pozicija koja se koristi za detekciju van terena
        bool Detected;
        int Id;
    };
    Enemy detectedEnemies[2];

    /* Ignoring outer field obsticles*/
    int maxX;
    int maxY;
    int minX;
    int minY;
    int margin;
    bool checkField;
    bool isInField(Point2D& enemyPosition);

    /* for slowing down motion on small distances */
    bool shouldUseSlow(int distance, int originalSpeed);
    bool shouldUseSlow(geometry::Point2D _position, int originalSpeed);

    mutex pfLock;
    PathFinding* pathFinder;
    int dodajSestougao(int krugX, int krugY, int triangleSide, int angle);
    int dodajCustomOblik(int krugX, int krugY, int angle);
    int enemyDistance;
    int triangleSide;
    struct EnemyDimensions{
        int Distance;
        int Front;
        int Back;
        int SideFront;
        int SideBack;
    };
    EnemyDimensions enemyDimension;
};

}

#endif //_MOTIONEXECUTOR_H
