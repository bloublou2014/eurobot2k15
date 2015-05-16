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

string BeaconNotification::NAME = "BeaconNotification";

void BeaconNotification::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate);
    tpl->SetClassName(String::NewFromUtf8(isolate, BeaconNotification::NAME.c_str()));
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "maliCordX"),
                            MaliCordXGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "maliCordY"),
                            MaliCordYGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "velikiCordX"),
                            VelikiCordXGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "velikiCordY"),
                            VelikiCordYGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "maliValidData"),
                            MaliValidDataGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "velikiValidData"),
                            VelikiValidDataGetter);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(BeaconNotification::NAME, tpl->GetFunction());
    provider->setObjectTemplate(BeaconNotification::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, BeaconNotification::NAME.c_str()), tpl->GetFunction());
}

void BeaconNotification::MaliCordXGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    BeaconNotification* notification=Unwrap<BeaconNotification>(self);
    info.GetReturnValue().Set(notification->getMaliCordX());
}

void BeaconNotification::MaliCordYGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    BeaconNotification* notification=Unwrap<BeaconNotification>(self);
    info.GetReturnValue().Set(notification->getMaliCordY());
}

void BeaconNotification::VelikiCordXGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    BeaconNotification* notification=Unwrap<BeaconNotification>(self);
    info.GetReturnValue().Set(notification->getVelikiCordX());
}

void BeaconNotification::VelikiCordYGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    BeaconNotification* notification=Unwrap<BeaconNotification>(self);
    info.GetReturnValue().Set(notification->getVelikiCordY());
}

void BeaconNotification::MaliValidDataGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    BeaconNotification* notification=Unwrap<BeaconNotification>(self);
    info.GetReturnValue().Set(notification->getMaliValidData());
}

void BeaconNotification::VelikiValidDataGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    BeaconNotification* notification=Unwrap<BeaconNotification>(self);
    info.GetReturnValue().Set(notification->getVelikiValidData());
}

}
