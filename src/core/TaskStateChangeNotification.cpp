#include "TaskStateChangeNotification.h"

namespace robot{

string TaskStateChangeNotification::NAME="TaskStateChange";
void TaskStateChangeNotification::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate);
    tpl->SetClassName(String::NewFromUtf8(isolate, TaskStateChangeNotification::NAME.c_str()));
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "state"),
                            TaskStateGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "taskName"),
                            TaskNameGetter);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(TaskStateChangeNotification::NAME.c_str(),tpl->GetFunction());
    provider->setObjectTemplate(TaskStateChangeNotification::NAME.c_str(), tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, TaskStateChangeNotification::NAME.c_str()), tpl->GetFunction());
}

void TaskStateChangeNotification::TaskNameGetter(Local<String> property, const PropertyCallbackInfo<Value>& info) {
    Local<Object> self = info.Holder();
    TaskStateChangeNotification* command=Unwrap<TaskStateChangeNotification>(self);
    string value = command->task;
    info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), value.c_str()));
}

void TaskStateChangeNotification::TaskStateGetter(Local<String> property, const PropertyCallbackInfo<Value>& info) {
    Local<Object> self = info.Holder();
    TaskStateChangeNotification* command=Unwrap<TaskStateChangeNotification>(self);
    int value = command->state;
    info.GetReturnValue().Set(value);
}

}
