#ifndef JSSCHEDULER_H
#define JSSCHEDULER_H

#include "core/Scheduler.h"

using std::vector;

namespace robot{

class JSScheduler: public Scheduler{
public:
    JSScheduler(const string& _scriptName, TaskManagerInterface *handler);

    void notificationReceived(Notification* testNotification);
protected:
    void onCreate();
    void onStart();
    void onStop();
    void onStateUpdate(AbstractTask *task, TaskState oldState, TaskState newState);

    void createGlobalObjects(Handle<Object> global);
    void getScriptNames(std::vector<string>& scripts);
    static JSScheduler* UnwrapJSScheduler(Handle<Object> object, int i=0);

    static Handle<ObjectTemplate> createLogTemplate(Isolate* isolate);
    static Handle<ObjectTemplate> createNotificationTemplate(Isolate* isolate);
    static Handle<ObjectTemplate> createSchedulerTemplate(Isolate* isolate);

    static void debugCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void subscripbeCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
private:
    string scriptName;

    void wrapTasks(Handle<Object> global);

    Persistent<Function> runCallback;
    Persistent<Function> stopCallback;
    Persistent<Function> stateUpdateCallback;

    Persistent<ObjectTemplate> loggerTemplate;
    Persistent<ObjectTemplate> notificationTemplate;
    Persistent<ObjectTemplate> schedulerTemplate;

    map<string, Persistent<Function>> subscribedFunctions;

    static map<TaskState, Persistent<String>> taskStateConstants;

    static string TASK_NAME;
    Persistent<ObjectTemplate> taskTemplate;
    static void StateGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void GetRunningTask(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void StartTask(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void PauseActiveTask(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void setWorldStateCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getWorldStateCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void sendCancelNotificationCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}
#endif // JSSCHEDULER_H
