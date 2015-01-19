#ifndef _JAVASCRIPTTASK_H
#define _JAVASCRIPTTASK_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

#include <include/v8.h>
#include <include/libplatform/libplatform.h>

#include "AbstractTask.h"

using namespace v8;
using boost::mutex;
using boost::unique_lock;

namespace robot{

class JavaScriptTask : public AbstractTask{
public:
    JavaScriptTask(const string& _name, string _scriptName):AbstractTask(_name), scriptName(_scriptName){}

    static void InitV8Platform();
protected:

    //Task callbacks
    void onCreate();
    void onRun();
    void onPause();
    void onDestroy();

    static void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch);
    static const char* ToCString(const v8::String::Utf8Value& value);
    static Handle<String> ReadScript(Isolate* isolate, const string& fileName);
    static Handle<ObjectTemplate> createLogTemplate(Isolate* isolate);
    static Logger* UnwrapLogger(Handle<Object> object);

    //Callback from JS functions
    static void debugCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void warningCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void errorCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

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

    Isolate* isolate;
    Persistent<Context> taskContext;
};

}

#endif //_JAVASCRIPTTASK_H
