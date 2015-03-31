#ifndef _ABSTRACTTASK_H
#define _ABSTRACTTASK_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <queue>

#include "core/Node.h"
#include "core/CommandSource.h"
#include "core/NotificationHandler.h"
#include "core/NotificationSource.h"
#include "core/AbstractMessageHandler.h"

using std::queue;
using boost::mutex;
using boost::condition_variable;
using boost::unique_lock;

namespace robot{

//lowest prority states have lowest values
enum TaskState{
    IMPOSSIBLE=0,
    FINISHED=1,
    SUSPENDED=2,
    READY=3,
    RUNNING=4
};

class AbstractTask: public Node, protected CommandSource, protected NotificationHandler, public NotificationSource{
public:
    AbstractTask(const string& name):Node(name), taskKilled(false){}

    bool passMessage(Message* message);
    TaskState getTaskState() const;

    //task manager will call this when task's state changes to RUNNING
    void startTask();
    //task manager will call this when task changes from RUNNING
    void stopTask();
    //task manager will call this when task is terminated, task thread will be killed
    void killTask();

    //ovo bi trebao samo manager da poziva. Potrebno je atomicno promenuti stanje i updateovati heap.
    void setState(TaskState _state);

    void registerManager(AbstractMessageHandler* manager);
protected:
    struct Instruction{
        enum Type{
            MESSAGE,
            START,
            STOP,
            KILL
        };

        Instruction(Message* _message):message(_message),type(MESSAGE){}
        Instruction(Type _type):type(_type){}

        Message* message;
        Type type;
    };


    Instruction fetchInstruction();
    void main();

    //this will be called when task's thread is created and it should be used to subsribe for desired events
    virtual void initScript()=0;
    //this will notify task that it has been started (changed state to RUNNING)
    virtual void startScript()=0;
    //this will notify task that it has been stopped (changed state from RUNNING)
    virtual void stopScript()=0;

    //Override of CommandSource method, checking if task is premitted to send commands
    bool sendCommand(Command* command, responseCallback success, responseCallback error, responseCallback progress=NULL);
private:
    mutex queueLock;
    condition_variable queueNotEmpty;
    queue<Instruction> instructionQueue;

    TaskState state;
    bool taskKilled;

    AbstractMessageHandler* handler;
};

}

#endif //_ABSTRACTTASK_H
