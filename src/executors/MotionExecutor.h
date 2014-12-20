#ifndef _MOTIONEXECUTOR_H
#define _MOTIONEXECUTOR_H

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <queue>

#include "AbstractExecutor.h"

using boost::mutex;
using std::queue;

namespace robot{

class MotionExecutor: public AbstractExecutor{
public:
    static string NAME;
    ExampleExecutor():AbstractExecutor(NAME),currentCommand(NULL),counter(0),totalCounter(0){}

    void init();
    void stop();

    void processMotionCommand(Command* command);
protected:
    void main();
private:
    mutex commandQueueLock;
    queue<Command*> commandsToProcess;

    Command* ExecuteNextCommand();

    int totalCounter;
    int counter;

    bool shouldStop;

    Command* currentCommand;
};

}

#endif //_MOTIONEXECUTOR_H
