#ifndef _JAVASCRIPTTASK_H
#define _JAVASCRIPTTASK_H

#include <include/v8.h>
#include <include/libplatform/libplatform.h>

#include "AbstractTask.h"

using namespace v8;

namespace robot{

class JavaScriptTask : public AbstractTask{
public:
    JavaScriptTask(const string& _name, std::string _scriptName);

    static void InitV8Platform();
protected:

    void onCreate();
    void onRun();
    void onPause();
    void onDestroy();

    bool executeScript(Handle<String> script);
    bool executeGlobalFunction(Handle<Function> function, const int argc, Handle<Value> argv[]);

    Isolate* getIsolate() const;
    Persistent<String> script;
    Persistent<Context> taskContext;

    //Global callback functions
    Persistent<Function> runCallback;
    Persistent<Function> pauseCallback;

    static void debugCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void warningCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void errorCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

private:
    std::string scriptName;
    Handle<String> loadScript(const string &name);

    Isolate* isolate;
};

}

#endif //_JAVASCRIPTTASK_H
