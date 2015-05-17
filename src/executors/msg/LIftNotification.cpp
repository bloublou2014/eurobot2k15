#include "LIftNotification.h"

namespace executor{

string LIftNotification::NAME="LiftNotification";
void LIftNotification::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate);
    tpl->SetClassName(String::NewFromUtf8(isolate, LIftNotification::NAME.c_str()));
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "side"),
                            SideGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "count"),
                            CountGetter);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(LIftNotification::NAME, tpl->GetFunction());
    provider->setObjectTemplate(LIftNotification::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, LIftNotification::NAME.c_str()), tpl->GetFunction());
}

void LIftNotification::SideGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    LIftNotification* notification=Unwrap<LIftNotification>(self);
    info.GetReturnValue().Set(notification->getSide());
}

void LIftNotification::CountGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    LIftNotification* notification=Unwrap<LIftNotification>(self);
    info.GetReturnValue().Set(notification->getCount());
}

string LiftProgressNotification::NAME="LiftProgressNotification";
void LiftProgressNotification::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate);
    tpl->SetClassName(String::NewFromUtf8(isolate, LiftProgressNotification::NAME.c_str()));
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "type"),
                            LiftProgressNotification::TypeGetter);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(LiftProgressNotification::NAME, tpl->GetFunction());
    provider->setObjectTemplate(LiftProgressNotification::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, LiftProgressNotification::NAME.c_str()), tpl->GetFunction());
}

void LiftProgressNotification::TypeGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    LiftProgressNotification* progress=Unwrap<LiftProgressNotification>(self);
    info.GetReturnValue().Set((int)progress->getType());
}

}
