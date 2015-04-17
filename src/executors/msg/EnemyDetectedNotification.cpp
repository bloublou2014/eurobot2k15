#include "EnemyDetectedNotification.h"

namespace executor{

string EnemyDetectedNotification::NAME="EnemyDetectedNotification";
void EnemyDetectedNotification::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate);
    tpl->SetClassName(String::NewFromUtf8(isolate, EnemyDetectedNotification::NAME.c_str()));
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "type"),
                            TypeGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "angle"),
                            AngleGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "detected"),
                            DetectedGetter);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(EnemyDetectedNotification::NAME, tpl->GetFunction());
    provider->setObjectTemplate(EnemyDetectedNotification::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, EnemyDetectedNotification::NAME.c_str()), tpl->GetFunction());
}

void EnemyDetectedNotification::TypeGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    EnemyDetectedNotification* notification=Unwrap<EnemyDetectedNotification>(self);
    info.GetReturnValue().Set(notification->getType());
}

void EnemyDetectedNotification::AngleGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    EnemyDetectedNotification* notification=Unwrap<EnemyDetectedNotification>(self);
    info.GetReturnValue().Set(notification->getAngle());
}

void EnemyDetectedNotification::DetectedGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    EnemyDetectedNotification* notification=Unwrap<EnemyDetectedNotification>(self);
    info.GetReturnValue().Set(notification->isDetected());
}

}
