#include "JavaScriptTask.h"

namespace robot{

string JavaScriptTask::UTILS_SCRIPT="utils.js";

JavaScriptTask* JavaScriptTask::UnwrapJavascriptTask(Handle<Object> object){
    Handle<External> field = Handle<External>::Cast(object->GetInternalField(0));
    void* ptr = field->Value();
    return static_cast<JavaScriptTask*>(ptr);
}

void JavaScriptTask::createGlobalObjects(Handle<Object> global){
    HandleScope scope(getIsolate());

    getIsolate()->SetData(1,this);

    /* Exposing builtin functions and objects */
    Local<Object> logger=createObjectFromTemplate(static_cast<ObjectTemplateBuilder>(&createLogTemplate), loggerTemplate, this);
    Local<Object> notification=createObjectFromTemplate(static_cast<ObjectTemplateBuilder>(&createNotificationTemplate), notificationTemplate, this);
    Local<Object> command=createObjectFromTemplate(static_cast<ObjectTemplateBuilder>(&createCommandTemplate), commandTemplate, this);
    Local<Object> manager=createObjectFromTemplate(static_cast<ObjectTemplateBuilder>(&createManagerTemplate), managerTemplate, this);
    global->Set(v8::String::NewFromUtf8(getIsolate(), "Logger"), logger);
    global->Set(v8::String::NewFromUtf8(getIsolate(), "Notification"), notification);
    global->Set(v8::String::NewFromUtf8(getIsolate(), "Command"), command);
    global->Set(v8::String::NewFromUtf8(getIsolate(), "Manager"), manager);
}

void JavaScriptTask::getScriptNames(std::vector<string> &scripts){
    scripts.push_back(directoryName+boost::filesystem::path::preferred_separator+UTILS_SCRIPT);
    scripts.push_back(scriptName);
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
    result->Set(String::NewFromUtf8(isolate, "getColor", String::kInternalizedString),
                FunctionTemplate::New(isolate, getCollorCallback));
    result->Set(String::NewFromUtf8(isolate, "getWorldState", String::kInternalizedString),
                FunctionTemplate::New(isolate, getWorldStateCallback));
    result->Set(String::NewFromUtf8(isolate, "setWorldState", String::kInternalizedString),
                FunctionTemplate::New(isolate, setWorldStateCallback));

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

void JavaScriptTask::onCreate(){
    debug("onCreate");

    //Initialize VM
    initializeGlobalState();

    HandleScope scope(getIsolate());


    v8::Local<v8::Context> context =
                v8::Local<v8::Context>::New(getIsolate(), taskContext);
    Context::Scope contextScope(context);

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

void JavaScriptTask::onRun(){
    callJavascriptCommandCallback(runCallback,NULL);
}

void JavaScriptTask::onPause(){
    callJavascriptCommandCallback(pauseCallback,NULL);}

void JavaScriptTask::onDestroy(){
    debug("Exiting isolate");
    finalize();
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
    currentVM->debug("Hello world");
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

void JavaScriptTask::getCollorCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
    Isolate* isolate=Isolate::GetCurrent();
    JavaScriptTask* task=UnwrapJavascriptTask(args.Holder());

    StartMessage::Color matchColor;
    if (task->getColor(matchColor)){
        if (matchColor==StartMessage::Color::GREEN)
            args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), "GREEN"));
        else
            args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), "YELLOW"));
    }else{
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Can't get color, match not started!"));
    }
}

void JavaScriptTask::setWorldStateCallback(const v8::FunctionCallbackInfo<Value> &args){
    Isolate* isolate=Isolate::GetCurrent();
    JavaScriptTask* task=UnwrapJavascriptTask(args.Holder());

    if (args.Length()<2){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Not enaugh parameters!"));
    }

    v8::String::Utf8Value key(args[0]);
    v8::String::Utf8Value value(args[1]);

    task->getHandler()->setWorldProperty(ToCString(key),ToCString(value));
    args.GetReturnValue().Set(true);
}

void JavaScriptTask::getWorldStateCallback(const v8::FunctionCallbackInfo<Value> &args){
    Isolate* isolate=Isolate::GetCurrent();
    JavaScriptTask* task=UnwrapJavascriptTask(args.Holder());

    if (args.Length()<1){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Not enaugh parameters!"));
    }

    v8::String::Utf8Value key(args[0]);

    string value= task->getHandler()->getWorldProperty(ToCString(key));

    args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), value.c_str()));
}

}
