#include "JavaScriptTask.h"

namespace robot{

void JavaScriptTask::InitV8Platform(){
    v8::V8::InitializeICU();
    v8::Platform* platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);
    v8::V8::Initialize();
}

Handle<String> JavaScriptTask::loadScript(const string& name) {
    FILE* file = fopen(name.c_str(), "rb");
    if (file == NULL) return Handle<String>();

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    char* chars = new char[size + 1];
    chars[size] = '\0';

    for (int i = 0; i < size;) {
        int read = static_cast<int>(fread(&chars[i], 1, size - i, file));
        i += read;
    }

    fclose(file);
    Handle<String> result = String::NewFromUtf8(isolate, chars, String::kNormalString, size);
    delete[] chars;
    return result;
}

JavaScriptTask::JavaScriptTask(const string& _name, string _scriptName):AbstractTask(_name), scriptName(_scriptName){
    //Probably empty?
}

Isolate* JavaScriptTask::getIsolate() const{
    return isolate;
}

bool JavaScriptTask::executeScript(Handle<String> script){
    HandleScope scope(getIsolate());

    TryCatch tryCatch;
    Handle<Script> compiledScript=Script::Compile(script);
    if (compiledScript.IsEmpty()){
        error("Script failed to compile");
        //Script failed to compile
        return false;
    }

    Handle<Value> result=compiledScript->Run();
    if (result.IsEmpty()){
        error("Script failed to run");
        //Script failed to run
        return false;
    }

    return true;
}

void JavaScriptTask::onCreate(){
    debug("onCreate");

    isolate = Isolate::New();
    isolate->Enter();

    HandleScope scope(getIsolate());

    Handle<Context> context=Context::New(getIsolate());
    taskContext.Reset(getIsolate(), context);

    Context::Scope contextScope(context);

    Handle<String> source=loadScript(scriptName);
    script.Reset(getIsolate(), source);
    executeScript(source);

    Handle<String> runName=String::NewFromUtf8(getIsolate(),"onRun");
    Handle<String> pauseName=String::NewFromUtf8(getIsolate(), "onPause");

    Handle<Value> runValue=context->Global()->Get(runName);
    Handle<Value> pauseValue=context->Global()->Get(pauseName);

    if (!runValue->IsFunction() || !pauseValue->IsFunction()){
        //Cant initialize, no start and stop functions defined in script, throw exception
        return;
    }

    Handle<Function> run=Handle<Function>::Cast(runValue);
    Handle<Function> pause=Handle<Function>::Cast(pauseValue);

    runCallback.Reset(getIsolate(), run);
    pauseCallback.Reset(getIsolate(), pause);
}

bool JavaScriptTask::executeGlobalFunction(Handle<Function> function, const int argc, Handle<Value> argv[]){
    HandleScope scope(getIsolate());

    Local<Context> context=Local<Context>::New(getIsolate(),taskContext);
    Context::Scope contextScope(context);

    TryCatch tryCatch;

    Handle<Value> result=function->Call(context->Global(),argc,argv);

    if (result.IsEmpty()){
        error("Error starting script");
        return false;
    }

    return true;
}

void JavaScriptTask::onRun(){
    debug("onRun");

    Local<Function> start=Local<Function>::New(getIsolate(), runCallback);
    executeGlobalFunction(start,0,NULL);
}

void JavaScriptTask::onPause(){
    debug("onPause");

    Local<Function> stop=Local<Function>::New(getIsolate(), pauseCallback);
    executeGlobalFunction(stop,0,NULL);
}

void JavaScriptTask::onDestroy(){
    debug("Exiting isolate");
    isolate->Exit();
}

/*---- Callbacks from JS ---- */

void JavaScriptTask::debugCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    Handle<Value> arg = args[0];
    String::Utf8Value value(arg);
    printf("%s\n\n",*value);
}

void JavaScriptTask::warningCallback(const v8::FunctionCallbackInfo<v8::Value>& args){

}

void JavaScriptTask::errorCallback(const v8::FunctionCallbackInfo<v8::Value>& args){

}

}
