#include "JSScheduler.h"

namespace robot{

JSScheduler::JSScheduler(const string &_scriptName, TaskManagerInterface* handler):Scheduler(handler),
    scriptName(_scriptName){
}

void JSScheduler::notificationReceived(Notification *testNotification){
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

void JSScheduler::onCreate(){
    debug("onCreate");

    //Initialize VM
    initializeGlobalState();

    HandleScope scope(getIsolate());

    v8::Local<v8::Context> context =
                v8::Local<v8::Context>::New(getIsolate(), taskContext);
    Context::Scope contextScope(context);

    Handle<String> runName=String::NewFromUtf8(getIsolate(),"onRun");
    Handle<String> stopName=String::NewFromUtf8(getIsolate(), "onStop");
    Handle<String> stateUpdateName=String::NewFromUtf8(getIsolate(), "onStateUpdate");

    Handle<Value> runValue=context->Global()->Get(runName);
    Handle<Value> stopValue=context->Global()->Get(stopName);
    Handle<Value> stateUpdateValue=context->Global()->Get(stateUpdateName);

    if (!runValue->IsFunction() || !stopValue->IsFunction() || !stateUpdateValue->IsFunction()){
        throw TaskExecutionException("Invalid script. Scheduler must contain minimum onRun and onStop and onStateUpdate functions!");
    }

    Handle<Function> run=Handle<Function>::Cast(runValue);
    Handle<Function> pause=Handle<Function>::Cast(stopValue);
    Handle<Function> stateUpdte=Handle<Function>::Cast(stateUpdateValue);

    runCallback.Reset(getIsolate(), run);
    stopCallback.Reset(getIsolate(), pause);
    stateUpdateCallback.Reset(getIsolate(), stateUpdte);
}

void JSScheduler::onStart(){
    callJavascriptCommandCallback(runCallback,NULL);
}

void JSScheduler::onStop(){
    callJavascriptCommandCallback(stopCallback,NULL);
}

void JSScheduler::onStateUpdate(AbstractTask *task, TaskState oldState, TaskState newState){
    HandleScope scope(getIsolate());
    Local<Context> context =Local<Context>::New(getIsolate(), taskContext);
    Context::Scope contextScope(context);

    TryCatch tryCatch;

    Handle<Object> obj;
    int argc=3;
    Handle<Value> argv[argc];
    argv[0]=task->handle();
    argv[1]=Local<String>::New(getIsolate(),taskStateConstants[oldState]);
    argv[2]=Local<String>::New(getIsolate(),taskStateConstants[newState]);

    Local<Function> func=Local<Function>::New(getIsolate(), stateUpdateCallback);
    Handle<Value> result= func->Call(context->Global(),argc, argv);
    if (result.IsEmpty()){
        ReportException(getIsolate(), &tryCatch);
        throw JavaScriptVMException("Running callback method function failed.");
    }
}

void JSScheduler::createGlobalObjects(Handle<Object> global){
    HandleScope scope(getIsolate());

    getIsolate()->SetData(1,this);

    /* Exposing builtin functions and objects */
    Local<Object> logger=createObjectFromTemplate(static_cast<ObjectTemplateBuilder>(&createLogTemplate), loggerTemplate, this);
    Local<Object> notification=createObjectFromTemplate(static_cast<ObjectTemplateBuilder>(&createNotificationTemplate), notificationTemplate, this);
    Local<Object> scheduler=createObjectFromTemplate(static_cast<ObjectTemplateBuilder>(&createSchedulerTemplate), schedulerTemplate, this);
    global->Set(v8::String::NewFromUtf8(getIsolate(), "Logger"), logger);
    global->Set(v8::String::NewFromUtf8(getIsolate(), "Notification"), notification);
    global->Set(v8::String::NewFromUtf8(getIsolate(), "Scheduler"), scheduler);

    wrapTasks(global);
}

void JSScheduler::getScriptNames(std::vector<string> &scripts){
    scripts.push_back(scriptName);
}

JSScheduler *JSScheduler::UnwrapJSScheduler(Handle<Object> object, int i){
    Handle<External> field = Handle<External>::Cast(object->GetInternalField(i));
    void* ptr = field->Value();
    return static_cast<JSScheduler*>(ptr);
}

Handle<ObjectTemplate> JSScheduler::createLogTemplate(Isolate *isolate){
    EscapableHandleScope scope(isolate);

    Local<ObjectTemplate> result=ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    result->Set(String::NewFromUtf8(isolate, "debug", String::kInternalizedString),
                FunctionTemplate::New(isolate,debugCallback));

    return scope.Escape(result);
}

Handle<ObjectTemplate> JSScheduler::createNotificationTemplate(Isolate *isolate){
    EscapableHandleScope scope(isolate);

    Local<ObjectTemplate> result=ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    result->Set(String::NewFromUtf8(isolate, "subscribe", String::kInternalizedString),
                FunctionTemplate::New(isolate, subscripbeCallback));

    return scope.Escape(result);
}

Handle<ObjectTemplate> JSScheduler::createSchedulerTemplate(Isolate *isolate){
    EscapableHandleScope scope(isolate);

    Local<ObjectTemplate> result=ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    result->Set(String::NewFromUtf8(isolate, "getRunningTask", String::kInternalizedString),
                FunctionTemplate::New(isolate, GetRunningTask));
    result->Set(String::NewFromUtf8(isolate, "startTask", String::kInternalizedString),
                FunctionTemplate::New(isolate, StartTask));
    result->Set(String::NewFromUtf8(isolate, "pauseActiveTask", String::kInternalizedString),
                FunctionTemplate::New(isolate, PauseActiveTask));
    result->Set(String::NewFromUtf8(isolate, "getWorldState", String::kInternalizedString),
                FunctionTemplate::New(isolate, getWorldStateCallback));
    result->Set(String::NewFromUtf8(isolate, "setWorldState", String::kInternalizedString),
                FunctionTemplate::New(isolate, setWorldStateCallback));

    return scope.Escape(result);
}

void JSScheduler::debugCallback(const v8::FunctionCallbackInfo<Value> &args){
    JSScheduler* logger=UnwrapJSScheduler(args.Holder());
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    Handle<Value> arg = args[0];
    if (arg->IsString()){
        String::Utf8Value value(arg);
        logger->debug(*value);
    }
}

void JSScheduler::subscripbeCallback(const v8::FunctionCallbackInfo<Value> &args){
    Isolate* isolate=Isolate::GetCurrent();

    if (args.Length()<2){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Not enaugh parameters."));
    }
    String::Utf8Value paramName(args[0]->ToString());
    string name(*paramName);

    JSScheduler* currentVM=static_cast<JSScheduler*>(isolate->GetData(1));
    currentVM->subscribedFunctions[name].Reset(isolate, args[1]);
    currentVM->subscribe(name,(notificationCallback)&JSScheduler::notificationReceived);
}

string JSScheduler::TASK_NAME="Task";
map<TaskState, Persistent<String>> JSScheduler::taskStateConstants;
void JSScheduler::wrapTasks(Handle<Object> global){
    Isolate* isolate = getIsolate();
    HandleScope scope(getIsolate());

    taskStateConstants[IMPOSSIBLE].Reset(isolate, v8::String::NewFromUtf8(Isolate::GetCurrent(), "Impossible"));
    taskStateConstants[FINISHED].Reset(isolate, v8::String::NewFromUtf8(Isolate::GetCurrent(), "Finished"));
    taskStateConstants[SUSPENDED].Reset(isolate, v8::String::NewFromUtf8(Isolate::GetCurrent(), "Suspended"));
    taskStateConstants[READY].Reset(isolate, v8::String::NewFromUtf8(Isolate::GetCurrent(), "Ready"));
    taskStateConstants[RUNNING].Reset(isolate, v8::String::NewFromUtf8(Isolate::GetCurrent(), "Running"));

    Local<ObjectTemplate> tpl = ObjectTemplate::New(isolate);
    tpl->SetAccessor(String::NewFromUtf8(isolate, "state"),
                            StateGetter);
    tpl->SetInternalFieldCount(1);

    Local<Object> taskMap=Object::New(isolate);

    //Setting objects
    Handle<String> rankName=String::NewFromUtf8(getIsolate(),"rank");
    Handle<String> taskNameLiteral=String::NewFromUtf8(getIsolate(),"name");
    vector<AbstractTask*>::iterator it=availableTasks.begin();
    for (;it!=availableTasks.end();++it){
        Local<Object> obj= tpl->NewInstance();
        AbstractTask* t=*it;
        Handle<Value> rank=Number::New(getIsolate(),t->getRank());
        obj->Set(rankName,rank);
        Handle<String> taskName=String::NewFromUtf8(getIsolate(),t->getName().c_str());
        obj->Set(taskNameLiteral,taskName);
        t->Wrap(obj);
        taskMap->Set(taskName,obj);
    }
    Handle<String> taskMapNameLiteral=String::NewFromUtf8(getIsolate(),"Tasks");
    global->Set(taskMapNameLiteral,taskMap);
}

void JSScheduler::StateGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Isolate* isolate=Isolate::GetCurrent();
    Local<Object> self = info.Holder();
    AbstractTask* task=AbstractTask::Unwrap<AbstractTask>(self);
    TaskState ts=task->getTaskState();
    Local<String> state=Local<String>::New(isolate,taskStateConstants[ts]);
    info.GetReturnValue().Set(state);
}

void JSScheduler::GetRunningTask(const v8::FunctionCallbackInfo<Value> &args){
    Isolate* isolate=Isolate::GetCurrent();
    JSScheduler* scheduler=static_cast<JSScheduler*>(isolate->GetData(1));
    AbstractTask* activeTask=scheduler->getActiveTask();
    if (activeTask==NULL){
        Local<Value> v;
        args.GetReturnValue().Set(v);
    }else{
        args.GetReturnValue().Set(activeTask->handle());
    }
}

void JSScheduler::StartTask(const v8::FunctionCallbackInfo<Value> &args){
    Isolate* isolate=Isolate::GetCurrent();
    JSScheduler* scheduler=static_cast<JSScheduler*>(isolate->GetData(1));
    if (!scheduler->hasStarted()){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Match hasn't started! Can't run task!"));
    }
    if (args.Length()<1){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong parameters"));
    }
    if (!args[0]->IsObject()){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong parameters, parameter must be task object"));
    }
    AbstractTask* task=AbstractTask::Unwrap<AbstractTask>(args[0]);
    if (scheduler->getActiveTask()!=NULL){
        scheduler->getActiveTask()->pauseTask();
    }
    scheduler->setActiveTask(task);
    task->runTask();
    args.GetReturnValue().Set(true);
}

void JSScheduler::PauseActiveTask(const v8::FunctionCallbackInfo<Value> &args){
    Isolate* isolate=Isolate::GetCurrent();
    JSScheduler* scheduler=static_cast<JSScheduler*>(isolate->GetData(1));
    if (scheduler->getActiveTask()!=NULL){
        scheduler->getActiveTask()->pauseTask();
        scheduler->setActiveTask(NULL);
        args.GetReturnValue().Set(true);
    }else{
        args.GetReturnValue().Set(false);
    }
}

void JSScheduler::setWorldStateCallback(const v8::FunctionCallbackInfo<Value> &args){
    Isolate* isolate=Isolate::GetCurrent();
    JSScheduler* scheduler=static_cast<JSScheduler*>(isolate->GetData(1));

    if (args.Length()<2){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Not enaugh parameters!"));
    }

    v8::String::Utf8Value key(args[0]);
    v8::String::Utf8Value value(args[1]);

    scheduler->getHandler()->setWorldProperty(ToCString(key),ToCString(value));
    args.GetReturnValue().Set(true);
}

void JSScheduler::getWorldStateCallback(const v8::FunctionCallbackInfo<Value> &args){
    Isolate* isolate=Isolate::GetCurrent();
    JSScheduler* scheduler=static_cast<JSScheduler*>(isolate->GetData(1));

    if (args.Length()<1){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Not enaugh parameters!"));
    }

    v8::String::Utf8Value key(args[0]);

    string value= scheduler->getHandler()->getWorldProperty(ToCString(key));

    args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), value.c_str()));
}

}
