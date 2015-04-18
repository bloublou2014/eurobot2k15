#ifndef _EXAMPLEEXECUTOR_H
#define _EXAMPLEEXECUTOR_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <queue>
#include <list>

#include "AbstractExecutor.h"
#include "msg/TimePassed.h"
#include "msg/CountdownCommand.h"
#include "msg/MotionNotification.h"
#include "msg/GetMotionState.h"
#include "utils/helper/SleepTimer.h"

using boost::mutex;
using std::list;
using std::queue;
using motion::MotionNotification;
using motion::GetMotionStateResponse;
using helper::SleepTimer;
using helper::TimerCallback;
using boost::condition_variable;
using boost::unique_lock;

namespace robot{

class ExampleExecutor: public AbstractExecutor, public TimerCallback{
public:
    static string NAME;
    ExampleExecutor():AbstractExecutor(NAME),counter(0),totalCounter(0){}

    void init();
    void countdown(Command* command);

    void stop();

    void onTimeout(const boost::system::error_code &e, void* obj);
protected:
    struct Instruction{
        enum Type{
            COMMAND,
            STOP
        };

        Instruction(Command* _command):command(_command),type(COMMAND){}
        Instruction(Type _type):type(_type),command(NULL){}

        Command* command;
        Type type;
    };

    void main();
private:
    mutex commandQueueLock;
    queue<Instruction> commandsToProcess;
    condition_variable queueNotEmpty;

    Instruction getNextInstruction();

    int totalCounter;
    int counter;

    bool shouldStop;

    list<SleepTimer*> timers;
};

}

#endif //_EXAMPLEEXECUTOR_H
