#ifndef TASKSTATECHANGENOTIFICATION_H
#define TASKSTATECHANGENOTIFICATION_H

#include "messages/Notification.h"
#include "utils/javascript/JavaScriptMessageProvider.h"
#include "utils/javascript/ObjectWrap.h"

using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;

namespace robot{

class TaskStateChangeNotification : public Notification
{
public:
    enum State{
        FINISHED=1
    };

    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    /* Getters and setters */
    static void TaskNameGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void TaskStateGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);

    TaskStateChangeNotification(const string& _task, State _state):Notification(NAME,"TaskManager"),
        task(_task), state(_state){}
    TaskStateChangeNotification(const TaskStateChangeNotification& tpn):Notification(tpn),
        task(tpn.task),state(tpn.state){}
    string getTaskName() const;
    string getTaskState() const;

    Message* clone(){
        return new TaskStateChangeNotification(*this);
    }

private:
    string task;
    State state;
};

}
#endif // TASKSTATECHANGENOTIFICATION_H
