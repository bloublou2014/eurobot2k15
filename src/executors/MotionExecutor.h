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

#include "AbstractExecutor.h"
#include "executors/msg/MotionCommand.h"
#include "executors/msg/GetMotionState.h"
#include "drivers/motion/MotionDriver.h"
#include "executors/msg/MotionNotification.h"
#include "executors/msg/EnemyDetectedNotification.h"

using namespace robot;
using boost::mutex;
using std::queue;

using executor::EnemyDetectedNotification;

namespace motion{

class MotionExecutor: public AbstractExecutor{
public:
    static string NAME;
    MotionExecutor():AbstractExecutor(NAME),currentMotionCommand(NULL){}

    void init();
    void stop();

    void processMotionCommand(Command* command);
    void processGetMotionState(Command* command);

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

    MotionCommand* currentMotionCommand;
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

    //Used for stuck detection
    MotionState previousState;
    static bool isStuck(MotionState& oldState, MotionState& newState);
    static double distance(double xFirst, double yFirst, double xSecond, double ySecond);

    /*Used for enemy detector*/
    bool detectedSensor[5];


};

}

#endif //_MOTIONEXECUTOR_H
