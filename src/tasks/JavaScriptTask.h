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

    void onCreate();
    void onRun();
    void onPause();
    void onDestroy();

    static void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch);
    static const char* ToCString(const v8::String::Utf8Value& value);

    static Handle<String> ReadScript(Isolate* isolate, const string& fileName);
    static Handle<ObjectTemplate> createLogTemplate(Isolate* isolate);
    static JavaScriptTask* UnwrapLogger(Handle<Object> object);

    Handle<Script> compileScript(Handle<String> scriptSource);
    void runScript(Handle<Script> compiledScript);
    bool executeGlobalFunction(Handle<Function> function, const int argc, Handle<Value> argv[]);

    void createGlobalObjects();

    inline Isolate* getIsolate() const;
    Persistent<Context> taskContext;

    //Global callback functions
    Persistent<Function> runCallback;
    Persistent<Function> pauseCallback;

    static void debugCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void warningCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void errorCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

private:
    std::string scriptName;

    Persistent<ObjectTemplate> loggerTemplate;

    Isolate* isolate;
};

}

#endif //_JAVASCRIPTTASK_H
