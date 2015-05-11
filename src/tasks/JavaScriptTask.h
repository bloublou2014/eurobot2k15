#ifndef _JAVASCRIPTTASK_H
#define _JAVASCRIPTTASK_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

#include <include/v8.h>
#include <include/libplatform/libplatform.h>

#include "AbstractTask.h"
#include "utils/javascript/JavaScriptMessageFactory.h"
#include "executors/msg/CountdownCommand.h"
#include "core/JavaScriptVM.h"

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

class JavaScriptTask : public AbstractTask, public JavaScriptVM{
public:
    static string UTILS_SCRIPT;

    JavaScriptTask(const string& _name, string _scriptName, const string& _directory, int _rank):
        AbstractTask(_name, _rank), scriptName(_scriptName), directoryName(_directory){}

    //Notification received callbacks
    void notificationReceived(Notification* testNotification);
    //Command response callbacks
    void commandSuccess(CommandResponse* resp);
    void commandError(CommandResponse* resp);
    void commandProgress(CommandResponse* resp);
protected:

    //Task callbacks
    void onCreate();
    void onSetup(StartMessage::Color matchColor);
    void onRun();
    void onPause();
    void onDestroy();

    //Helper functions for exposing objects in javascript
    static Handle<ObjectTemplate> createLogTemplate(Isolate* isolate);
    static Handle<ObjectTemplate> createCommandTemplate(Isolate* isolate);
    static Handle<ObjectTemplate> createNotificationTemplate(Isolate* isolate);
    static Handle<ObjectTemplate> createManagerTemplate(Isolate* isolate);

    //Logger callback functions
    static void debugCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void warningCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void errorCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    //Command callback functions
    static void sendCommandCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    //Notification callback functions
    static void subscripbeCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void unsubscribeCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    //Manager callbacks
    static void setStateCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getCollorCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void setWorldStateCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getWorldStateCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

    void createGlobalObjects(Handle<Object> global);
    void getScriptNames(std::vector<string>& scripts);

    static JavaScriptTask* UnwrapJavascriptTask(Handle<Object> object);
private:
    std::string scriptName;
    std::string directoryName;

    //Global callback functions
    Persistent<Function> runCallback;
    Persistent<Function> pauseCallback;
    Persistent<Function> setupCallback;

    Persistent<ObjectTemplate> loggerTemplate;
    Persistent<ObjectTemplate> commandTemplate;
    Persistent<ObjectTemplate> notificationTemplate;
    Persistent<ObjectTemplate> managerTemplate;

    map<string, Persistent<Function>> subscribedFunctions;
    map<int, CommandResponseCallback*> commandResponseCallbacks;
};

}

#endif //_JAVASCRIPTTASK_H
