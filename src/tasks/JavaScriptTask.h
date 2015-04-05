#ifndef _JAVASCRIPTTASK_H
#define _JAVASCRIPTTASK_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

#include <include/v8.h>
#include <include/libplatform/libplatform.h>

#include "AbstractTask.h"
#include "utils/javascript/JavaScriptMessageFactory.h"
#include "executors/msg/CountdownCommand.h"

using namespace v8;
using javascript::JavaScriptMessageFactory;

using boost::mutex;
using boost::unique_lock;

namespace robot{

struct CommandResponseCallback{
    Persistent<Function> success;
    Persistent<Function> error;
    Persistent<Function> progress;
};

class JavaScriptTask : public AbstractTask{
public:
    JavaScriptTask(const string& _name, string _scriptName):AbstractTask(_name), scriptName(_scriptName){}

    static void InitV8Platform();

    //Notification received callbacks
    void notificationReceived(Notification* testNotification);
    //Command response callbacks
    void commandSuccess(CommandResponse* resp);
    void commandError(CommandResponse* resp);
    void commandProgress(CommandResponse* resp);
protected:

    //Task callbacks
    void onCreate();
    void onRun();
    void onPause();
    void onDestroy();

    static void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch);
    static const char* ToCString(const v8::String::Utf8Value& value);
    static Handle<String> ReadScript(Isolate* isolate, const string& fileName);

    //Helper functions for exposing objects in javascript
    typedef Handle<ObjectTemplate> (*ObjectTemplateBuilder)(Isolate* isolate);
    static Handle<ObjectTemplate> createLogTemplate(Isolate* isolate);
    static Handle<ObjectTemplate> createCommandTemplate(Isolate* isolate);
    static Handle<ObjectTemplate> createNotificationTemplate(Isolate* isolate);
    static Handle<ObjectTemplate> createManagerTemplate(Isolate* isolate);

    Handle<Object> createObjectFromTemplate(ObjectTemplateBuilder builder, Persistent<ObjectTemplate>& objTemplate, void* internalField);

    static JavaScriptTask* UnwrapJavascriptTask(Handle<Object> object);

    //Logger callback functions
    static void debugCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void warningCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void errorCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    void callJavascriptCommandCallback(Persistent<Function>& function, CommandResponse* resp);
    //Command callback functions
    static void sendCommandCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    //Notification callback functions
    static void subscripbeCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void unsubscribeCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    //Manager callbacks
    static void setStateCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

    void createGlobalObjects();
    Handle<Script> compileScript(Handle<String> scriptSource);
    void runScript(Handle<Script> compiledScript);

    bool executeGlobalFunction(Handle<Function> function, const int argc, Handle<Value> argv[]);

    inline Isolate* getIsolate() const;
private:
    std::string scriptName;

    //Global callback functions
    Persistent<Function> runCallback;
    Persistent<Function> pauseCallback;

    Persistent<ObjectTemplate> loggerTemplate;
    Persistent<ObjectTemplate> commandTemplate;
    Persistent<ObjectTemplate> notificationTemplate;
    Persistent<ObjectTemplate> managerTemplate;

    Isolate* isolate;
    Persistent<Context> taskContext;
    map<string, Persistent<Function>> subscribedFunctions;
    map<int, CommandResponseCallback*> commandResponseCallbacks;

    JavaScriptMessageFactory* messageFactory;
};

}

#endif //_JAVASCRIPTTASK_H
