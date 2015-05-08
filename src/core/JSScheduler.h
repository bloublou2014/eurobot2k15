#ifndef JSSCHEDULER_H
#define JSSCHEDULER_H

#include <vector>

#include "core/Logger.h"
#include "core/NotificationHandler.h"
#include "core/JavaScriptVM.h"

#include "tasks/AbstractTask.h"

using std::vector;

namespace robot{

class JSScheduler:public NotificationHandler, public Logger, public JavaScriptVM{
public:
    JSScheduler(string _scriptName, const string& _directory);

    void addTask(AbstractTask* task);

    bool passMessage(Message* message);
    bool isSubscribed(Notification *message);


    void init();
    void start();
    void stop();
protected:
    struct Instruction{
        enum Type{
            MESSAGE,
            START,
            STOP
        };

        Instruction(Message* _message):message(_message),type(MESSAGE){}
        Instruction(Type _type):type(_type){}

        Message* message;
        Type type;
    };

    Instruction fetchInstruction();
private:
    mutex queueLock;
    condition_variable queueNotEmpty;
    queue<Instruction> instructionQueue;

    vector<AbstractTask*> availableTasks;

    AbstractTask* activeTask;
};

}
#endif // JSSCHEDULER_H
