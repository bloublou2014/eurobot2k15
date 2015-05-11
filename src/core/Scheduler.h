#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>

#include "core/Logger.h"
#include "core/NotificationHandler.h"
#include "core/JavaScriptVM.h"
#include "core/Node.h"

#include "tasks/AbstractTask.h"

using std::vector;

namespace robot{


class Scheduler:public Node, public NotificationHandler, public JavaScriptVM{
public:
    Scheduler(TaskManagerInterface* _hanler);

    void addTask(AbstractTask* task);

    bool passMessage(Message* message);
    bool isSubscribed(Notification *message);

    void startDispatching();
    void stop();

    void updateTaskState(AbstractTask *task, TaskState oldState, TaskState newState);
protected:
    struct Instruction{
        enum Type{
            MESSAGE,
            START,
            STOP,
            STATE_UPDATE
        };

        Instruction(Message* _message):message(_message),type(MESSAGE){}
        Instruction(Type _type):type(_type){}

        Message* message;
        Type type;
        //Used for state update instruction, not pretty but quickfix
        TaskState oldState;
        TaskState newState;
        AbstractTask* task;
    };

    Instruction fetchInstruction();
    void main();
    AbstractTask* getActiveTask() const;
    void setActiveTask(AbstractTask* task);

    virtual void onCreate()=0;
    virtual void onStart()=0;
    virtual void onStop()=0;
    virtual void onStateUpdate(AbstractTask *task, TaskState oldState, TaskState newState)=0;

    vector<AbstractTask*> availableTasks;
    bool hasStarted() const;

    TaskManagerInterface* getHandler() const;
private:
    bool started;
    bool shouldStop;
    TaskManagerInterface* handler;

    mutex queueLock;
    condition_variable queueNotEmpty;
    queue<Instruction> instructionQueue;

    AbstractTask* activeTask;
};

}
#endif // SCHEDULER_H
