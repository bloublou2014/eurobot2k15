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

Handle<Object> JavaScriptTask::createObjectFromTemplate(ObjectTemplateBuilder builder, Persistent<ObjectTemplate>& objTemplate, void* internalField){
    EscapableHandleScope scope(getIsolate());

    if (objTemplate.IsEmpty()){
        Handle<ObjectTemplate> newTemplate= builder(getIsolate());
        objTemplate.Reset(getIsolate(), newTemplate);
    }

    Handle<ObjectTemplate> templ=Local<ObjectTemplate>::New(getIsolate(), objTemplate);
    Local<Object> result=templ->NewInstance();
    Handle<External> refferencePtr=External::New(getIsolate(), internalField);
    result->SetInternalField(0, refferencePtr);

    return scope.Escape(result);
}

void JavaScriptTask::createGlobalObjects(){
    HandleScope scope(getIsolate());

    v8::Local<v8::Context> context =
            v8::Local<v8::Context>::New(getIsolate(), taskContext);
    Handle<Object> global=context->Global();

    /* Exposing builtin functions and objects */
    Local<Object> logger=createObjectFromTemplate(static_cast<ObjectTemplateBuilder>(&createLogTemplate), loggerTemplate, this);
    Local<Object> notification=createObjectFromTemplate(static_cast<ObjectTemplateBuilder>(&createNotificationTemplate), notificationTemplate, this);
    Local<Object> command=createObjectFromTemplate(static_cast<ObjectTemplateBuilder>(&createCommandTemplate), commandTemplate, this);
    Local<Object> manager=createObjectFromTemplate(static_cast<ObjectTemplateBuilder>(&createManagerTemplate), managerTemplate, this);
    global->Set(v8::String::NewFromUtf8(getIsolate(), "Logger"), logger);
    global->Set(v8::String::NewFromUtf8(getIsolate(), "Notification"), notification);
    global->Set(v8::String::NewFromUtf8(getIsolate(), "Command"), command);
    global->Set(v8::String::NewFromUtf8(getIsolate(), "Manager"), manager);

    messageFactory->init(global);
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

Isolate* JavaScriptTask::getIsolate() const{
    return isolate;
}

Handle<Script> JavaScriptTask::compileScript(Handle<String> scriptSource){
    EscapableHandleScope scope(getIsolate());

    TryCatch tryCatch;
    Local<Script> compiledScript=Script::Compile(scriptSource);
    if (compiledScript.IsEmpty()){
        ReportException(getIsolate(), &tryCatch);
        throw TaskExecutionException("Compiling script contains errors. Check stack trace for details.");
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

Handle<ObjectTemplate> JavaScriptTask::createCommandTemplate(Isolate* isolate){
    EscapableHandleScope scope(isolate);

    Local<ObjectTemplate> result=ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    result->Set(String::NewFromUtf8(isolate, "send", String::kInternalizedString),
                FunctionTemplate::New(isolate, sendCommandCallback));

    return scope.Escape(result);
}

Handle<ObjectTemplate> JavaScriptTask::createManagerTemplate(Isolate* isolate){
    EscapableHandleScope scope(isolate);

    Local<ObjectTemplate> result=ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    result->Set(String::NewFromUtf8(isolate, "updateState", String::kInternalizedString),
                FunctionTemplate::New(isolate, setStateCallback));

    return scope.Escape(result);
}

Handle<ObjectTemplate> JavaScriptTask::createNotificationTemplate(Isolate* isolate){
    EscapableHandleScope scope(isolate);

    Local<ObjectTemplate> result=ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    result->Set(String::NewFromUtf8(isolate, "subscribe", String::kInternalizedString),
                FunctionTemplate::New(isolate, subscripbeCallback));

    result->Set(String::NewFromUtf8(isolate, "unsubscribe", String::kInternalizedString),
                FunctionTemplate::New(isolate, unsubscribeCallback));

    return scope.Escape(result);
}

JavaScriptTask* JavaScriptTask::UnwrapJavascriptTask(Handle<Object> object){
    Handle<External> field = Handle<External>::Cast(object->GetInternalField(0));
    void* ptr = field->Value();
    return static_cast<JavaScriptTask*>(ptr);
}

void JavaScriptTask::runScript(Handle<Script> compiledScript){
    TryCatch tryCatch;
    Local<Value> result=compiledScript->Run();
    if (result.IsEmpty()){
        ReportException(getIsolate(), &tryCatch);
        throw TaskExecutionException("Running script contains errors. Check stack trace for details.");
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

    messageFactory=new JavaScriptMessageFactory();
    isolate->SetData(0,messageFactory);
    isolate->SetData(1,this);

    createGlobalObjects();

    Handle<String> script=ReadScript(getIsolate(), scriptName);
    if (script.IsEmpty()){
        throw TaskExecutionException("Error reading file. Empty or nonexisting file.");
    }

    Local<Script> compiledScript=compileScript(script);
    runScript(compiledScript);

    Handle<String> runName=String::NewFromUtf8(getIsolate(),"onRun");
    Handle<String> pauseName=String::NewFromUtf8(getIsolate(), "onPause");

    Handle<Value> runValue=context->Global()->Get(runName);
    Handle<Value> pauseValue=context->Global()->Get(pauseName);

    if (!runValue->IsFunction() || !pauseValue->IsFunction()){
        throw TaskExecutionException("Invalid script. Script must contain minumum onRun and onPause functions. Consult documentation.");
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
    callJavascriptCommandCallback(runCallback,NULL);
}

void JavaScriptTask::onPause(){
    callJavascriptCommandCallback(pauseCallback,NULL);}

void JavaScriptTask::onDestroy(){
    debug("Exiting isolate");
    isolate->Exit();
    delete messageFactory;
}

void JavaScriptTask::commandSuccess(CommandResponse* resp){
    if (commandResponseCallbacks.find(resp->getId())==commandResponseCallbacks.end()) return;
    callJavascriptCommandCallback(commandResponseCallbacks[resp->getId()]->success,resp);
    commandResponseCallbacks[resp->getId()]->success.Reset();
    commandResponseCallbacks[resp->getId()]->error.Reset();
    commandResponseCallbacks[resp->getId()]->progress.Reset();
    delete commandResponseCallbacks[resp->getId()];
}

void JavaScriptTask::commandError(CommandResponse* resp){
    if (commandResponseCallbacks.find(resp->getId())==commandResponseCallbacks.end()) return;
    callJavascriptCommandCallback(commandResponseCallbacks[resp->getId()]->error,resp);
    commandResponseCallbacks[resp->getId()]->success.Reset();
    commandResponseCallbacks[resp->getId()]->error.Reset();
    commandResponseCallbacks[resp->getId()]->progress.Reset();
    delete commandResponseCallbacks[resp->getId()];
}

void JavaScriptTask::commandProgress(CommandResponse* resp){
    if (commandResponseCallbacks.find(resp->getId())==commandResponseCallbacks.end()) return;
    callJavascriptCommandCallback(commandResponseCallbacks[resp->getId()]->progress,resp);
}

void JavaScriptTask::callJavascriptCommandCallback(Persistent<Function>& function, CommandResponse* resp){
    HandleScope scope(getIsolate());
    Local<Context> context =Local<Context>::New(getIsolate(), taskContext);
    Context::Scope contextScope(context);

    TryCatch tryCatch;

    Handle<Object> obj;
    int argc;
    Handle<Value> argv[argc];
    if (resp!=NULL){
        obj=messageFactory->wrapObject(resp->getName(),getIsolate(),resp);
        argc=1;
        argv[0]=obj;
    }else{
        argc=0;
    }

    Local<Function> func=Local<Function>::New(getIsolate(), function);
    Handle<Value> result= func->Call(context->Global(),argc, argv);
    if (result.IsEmpty()){
        ReportException(getIsolate(), &tryCatch);
        throw TaskExecutionException("Running callback method function failed.");
    }
}

void JavaScriptTask::notificationReceived(Notification* testNotification){
    HandleScope scope(getIsolate());
    Local<Context> context =Local<Context>::New(getIsolate(), taskContext);
    Context::Scope contextScope(context);

    TryCatch tryCatch;

    Handle<Object> obj=messageFactory->wrapObject(testNotification->getName(),getIsolate(),testNotification);

    int argc=1;
    Handle<Value> argv[argc];
    argv[0]=obj;

    Local<Function> func=Local<Function>::New(getIsolate(),subscribedFunctions[testNotification->getName()]);
    Handle<Value> result= func->Call(context->Global(),argc, argv);
    if (result.IsEmpty()){
        ReportException(getIsolate(), &tryCatch);
        throw TaskExecutionException("Running callback method function failed.");
    }
}

/*---- Callbacks from JS ---- */
//Logger functions
void JavaScriptTask::debugCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
    JavaScriptTask* logger=UnwrapJavascriptTask(args.Holder());
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    Handle<Value> arg = args[0];
    if (arg->IsString()){
        String::Utf8Value value(arg);
        logger->debug(*value);
    }
}

void JavaScriptTask::warningCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
    JavaScriptTask* logger=UnwrapJavascriptTask(args.Holder());
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    Handle<Value> arg = args[0];
    if (arg->IsString()){
        String::Utf8Value value(arg);
        logger->warning(*value);
    }
}

void JavaScriptTask::errorCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
    JavaScriptTask* logger=UnwrapJavascriptTask(args.Holder());
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    Handle<Value> arg = args[0];
    if (arg->IsString()){
        String::Utf8Value value(arg);
        logger->error(*value);
    }
}

//Notification functions
void JavaScriptTask::subscripbeCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
    Isolate* isolate=Isolate::GetCurrent();

    if (args.Length()<2){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
    String::Utf8Value paramName(args[0]->ToString());
    string name(*paramName);

    JavaScriptTask* currentVM=static_cast<JavaScriptTask*>(isolate->GetData(1));
    currentVM->subscribedFunctions[name].Reset(isolate, args[1]);
    currentVM->subscribe(name,(notificationCallback)&JavaScriptTask::notificationReceived);
}

void JavaScriptTask::unsubscribeCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
    Isolate* isolate=Isolate::GetCurrent();

    if (args.Length()<1){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Invalid argument number."));
    }
    String::Utf8Value paramName(args[0]->ToString());
    string name(*paramName);

    JavaScriptTask* currentVM=static_cast<JavaScriptTask*>(isolate->GetData(1));
    currentVM->subscribedFunctions[name].Reset();
    currentVM->unSubscribe(name);
}

void JavaScriptTask::sendCommandCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
    Isolate* isolate=Isolate::GetCurrent();

    if (args.Length()<3){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Not enaugh parameters."));
    }

    CommandResponseCallback* callback=new CommandResponseCallback();
    callback->success.Reset(isolate, args[1]);
    callback->error.Reset(isolate, args[2]);

    if (args.Length()>3){
        callback->progress.Reset(isolate,args[3]);
    }

    Local<Object> commandHandle=Local<Object>::Cast(args[0]);
    Command* command=static_cast<Command*>(ObjectWrap::Unwrap<Command>(commandHandle)->clone());

    int id;
    JavaScriptTask* currentVM=static_cast<JavaScriptTask*>(isolate->GetData(1));
    if (!callback->progress.IsEmpty()){
        id=currentVM->sendCommand(command,(responseCallback)&JavaScriptTask::commandSuccess,
                                  (responseCallback)&JavaScriptTask::commandError,
                                  (responseCallback)&JavaScriptTask::commandProgress);
    }else{
        id=currentVM->sendCommand(command,(responseCallback)&JavaScriptTask::commandSuccess,
                                  (responseCallback)&JavaScriptTask::commandError);
    }

    if (id==-1){
        currentVM->callJavascriptCommandCallback(callback->error, NULL);
    }else{
        currentVM->commandResponseCallbacks[id]=callback;
    }
}

//Manager functinos
void JavaScriptTask::setStateCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
    Isolate* isolate=Isolate::GetCurrent();
    JavaScriptTask* task=UnwrapJavascriptTask(args.Holder());
    if (args.Length() < 1){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "State not specified!"));
    }
    HandleScope scope(args.GetIsolate());
    Handle<Value> arg = args[0];
    if (arg->IsString()){
        String::Utf8Value value(arg);
        string state(*value);
        if (state=="Ready"){
            task->updateState(TaskState::READY);
            return;
        }else if(state=="Finished"){
            task->updateState(TaskState::FINISHED);
            return;
        }else if(state=="Impossible"){
            task->updateState(TaskState::IMPOSSIBLE);
            return;
        }else if(state=="Suspended"){
            task->updateState(TaskState::SUSPENDED);
            return;
        }
    }
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Unrecognized state"));
}

}
