#include "JavaScriptTask.h"

namespace robot{

void JavaScriptTask::InitV8Platform(){
    v8::V8::InitializeICU();
    v8::Platform* platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);
    v8::V8::Initialize();
}

const char* JavaScriptTask::ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

void JavaScriptTask::ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch) {
  static mutex exceptionLock;
    unique_lock<mutex> lock(exceptionLock);

    v8::HandleScope handle_scope(isolate);
  v8::String::Utf8Value exception(try_catch->Exception());
  const char* exception_string = ToCString(exception);
  v8::Handle<v8::Message> message = try_catch->Message();
  if (message.IsEmpty()) {
    // V8 didn't provide any extra information about this error; just
    // print the exception.
    fprintf(stderr, "%s\n", exception_string);
  } else {
    // Print (filename):(line number): (message).
    v8::String::Utf8Value filename(message->GetScriptOrigin().ResourceName());
    const char* filename_string = ToCString(filename);
    int linenum = message->GetLineNumber();
    fprintf(stderr, "%s:%i: %s\n", filename_string, linenum, exception_string);
    // Print line of source code.
    v8::String::Utf8Value sourceline(message->GetSourceLine());
    const char* sourceline_string = ToCString(sourceline);
    fprintf(stderr, "%s\n", sourceline_string);
    // Print wavy underline (GetUnderline is deprecated).
    int start = message->GetStartColumn();
    for (int i = 0; i < start; i++) {
      fprintf(stderr, " ");
    }
    int end = message->GetEndColumn();
    for (int i = start; i < end; i++) {
      fprintf(stderr, "^");
    }
    fprintf(stderr, "\n");
    v8::String::Utf8Value stack_trace(try_catch->StackTrace());
    if (stack_trace.length() > 0) {
      const char* stack_trace_string = ToCString(stack_trace);
      fprintf(stderr, "%s\n", stack_trace_string);
    }
  }
}

void JavaScriptTask::createGlobalObjects(){
    HandleScope scope(getIsolate());

    Handle<Object> global=taskContext->Global();

    if (loggerTemplate.IsEmpty()){
        Handle<ObjectTemplate> logger= createLogTemplate(getIsolate());
        loggerTemplate.Reset(getIsolate(), logger);
    }

    Handle<ObjectTemplate> templ=Local<ObjectTemplate>::New(getIsolate(),loggerTemplate);
    Local<Object> result=templ->NewInstance();
    Handle<External> loggerPtr=External::New(getIsolate(), this);

    result->SetInternalField(0, loggerPtr);

    global->Set(v8::String::NewFromUtf8(getIsolate(), "logger"), result);
}

Handle<String> JavaScriptTask::ReadScript(Isolate* isolate, const string& fileName) {
    FILE* file = fopen(fileName.c_str(), "rb");
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

JavaScriptTask* JavaScriptTask::UnwrapLogger(Handle<Object> object){
    Handle<External> field = Handle<External>::Cast(object->GetInternalField(0));
    void* ptr = field->Value();
    return static_cast<JavaScriptTask*>(ptr);
}

Isolate* JavaScriptTask::getIsolate() const{
    return isolate;
}

Handle<Script> JavaScriptTask::compileScript(Handle<String> scriptSource){
    EscapableHandleScope scope(getIsolate());

    TryCatch tryCatch;
    Local<Script> compiledScript=Script::Compile(scriptSource);
    if (compiledScript.IsEmpty()){
        error("Script failed to compile");
        //Script failed to compile, throw exception
        ReportException(getIsolate(), &tryCatch);
    }
    return scope.Escape(compiledScript);
}

Handle<ObjectTemplate> JavaScriptTask::createLogTemplate(Isolate* isolate){
    EscapableHandleScope scope(isolate);

    Local<ObjectTemplate> result=ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    result->Set(String::NewFromUtf8(isolate, "debug", String::kInternalizedString),
                FunctionTemplate::New(isolate,debugCallback));

    result->Set(String::NewFromUtf8(isolate, "error", String::kInternalizedString),
                FunctionTemplate::New(isolate,errorCallback));

    result->Set(String::NewFromUtf8(isolate, "warning", String::kInternalizedString),
                FunctionTemplate::New(isolate,warningCallback));

    return scope.Escape(result);
}

void JavaScriptTask::runScript(Handle<Script> compiledScript){
    TryCatch tryCatch;
    Local<Value> result=compiledScript->Run();
    if (result.IsEmpty()){
        error("Script failed to run");
        //Script failed to run, throw exception
        ReportException(getIsolate(), &tryCatch);
    }
}

void JavaScriptTask::onCreate(){
    debug("onCreate");

    isolate = Isolate::New();
    isolate->Enter();

    HandleScope scope(getIsolate());

    Handle<Context> context=Context::New(getIsolate());
    taskContext.Reset(getIsolate(), context);

    Context::Scope contextScope(context);

    CreateContext(context);

    Handle<String> script=ReadScript(getIsolate(), scriptName);
    if (script.IsEmpty()){
        //throw exception
        return;
    }

    Local<Script> compiledScript=compileScript(script);
    runScript(compiledScript);

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
    JavaScriptTask* logger=UnwrapLogger(args.Holder());
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    Handle<Value> arg = args[0];
    String::Utf8Value value(arg);
    logger->debug(*value);
}

void JavaScriptTask::warningCallback(const v8::FunctionCallbackInfo<v8::Value>& args){

}

void JavaScriptTask::errorCallback(const v8::FunctionCallbackInfo<v8::Value>& args){

}

}
