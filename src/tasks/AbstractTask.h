#ifndef _ABSTRACTTASK_H
#define _ABSTRACTTASK_H

#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include <queue>

#include "core/Node.h"
#include "core/CommandSource.h"
#include "core/NotificationHandler.h"

using std::queue;
using boost::mutex;
using boost::condition_variable;
using boost::unique_lock;

enum TaskState{
    READY,
    RUNNING,
    FINISHED,
    IMPOSSIBLE,
    SUSPENDED
};

namespace robot{

class AbstractTask: public Node, public CommandSource, public NotificationHandler {
public:
    AbstractTask(const string& name):Node(name), taskKilled(false){}

    bool passMessage(Message* message);
    TaskState getTaskState() const;

    virtual void startTask();
    virtual void stopTask();
    virtual void killTask();

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

    virtual void initScript()=0;
    virtual void startScript()=0;
    virtual void stopScript()=0;

    //Override of CommandSource method, adding checking if task is premitted to send commands
    bool sendCommand(Command* command, responseCallback success, responseCallback error, responseCallback progress=NULL);
private:
    mutex queueLock;
    condition_variable queueNotEmpty;
    queue<Instruction> instructionQueue;

    TaskState state;
    bool taskKilled;

    void setState(TaskState _state);
};

}

#endif //_ABSTRACTTASK_H
