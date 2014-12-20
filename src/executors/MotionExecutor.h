#ifndef _MOTIONEXECUTOR_H
#define _MOTIONEXECUTOR_H

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <queue>
#include <map>

#include "AbstractExecutor.h"
#include "executors/msg/MotionCommand.h"
#include "drivers/motion/MotionDriver.h"

using namespace robot;
using boost::mutex;
using std::queue;

namespace motion{

class MotionExecutor: public AbstractExecutor{
public:
    static string NAME;
    MotionExecutor():AbstractExecutor(NAME),currentMotionCommand(NULL){}

    void init();
    void stop();

    void processMotionCommand(Command* command);
protected:
    void main();
private:
    mutex commandQueueLock;
    queue<Command*> commandsToProcess;
    MotionCommand* getNextMotionCommand();

    bool shouldStop;

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
};

}

#endif //_MOTIONEXECUTOR_H
