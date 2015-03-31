#ifndef _EXAMPLEEXECUTOR_H
#define _EXAMPLEEXECUTOR_H

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <queue>

#include "AbstractExecutor.h"
#include "msg/TimePassed.h"
#include "msg/CountdownCommand.h"


using boost::mutex;
using std::queue;

namespace robot{

class ExampleExecutor: public AbstractExecutor{
public:
    static string NAME;
    ExampleExecutor():AbstractExecutor(NAME),currentCommand(NULL),counter(0),totalCounter(0){}

    void init();
    void processProbeEvent(Notification* notification);
    void countToN(Command* command);

    void stop();
protected:
    void main();
private:
    mutex commandQueueLock;
    queue<Command*> commandsToProcess;

    Command* ExecuteNextCommand();

    Command* currentCommand;
    int totalCounter;
    int counter = 0;

    bool shouldStop;
};

}

#endif //_EXAMPLEEXECUTOR_H
