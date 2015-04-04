#include "MotionNotification.h"

namespace motion{

string MotionNotification::NAME="MotionNotification";
void MotionNotification::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate);
    tpl->SetClassName(String::NewFromUtf8(isolate, MotionNotification::NAME.c_str()));
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "direction"),
                            DirectionGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "orientation"),
                            OrientationGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "x"),
                            PositionGetterX);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "y"),
                            PositionGetterY);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "speed"),
                            SpeedGetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "state"),
                            StateGetter);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(MotionNotification::NAME, tpl->GetFunction());
    provider->setObjectTemplate(MotionNotification::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, MotionNotification::NAME.c_str()), tpl->GetFunction());
}

void MotionNotification::DirectionGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    MotionNotification* notification=Unwrap<MotionNotification>(self);
    info.GetReturnValue().Set(notification->getState().Direction);
}

void MotionNotification::OrientationGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    MotionNotification* notification=Unwrap<MotionNotification>(self);
    info.GetReturnValue().Set(notification->getState().Orientation);
}

void MotionNotification::PositionGetterX(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    MotionNotification* notification=Unwrap<MotionNotification>(self);
    info.GetReturnValue().Set(notification->getState().Position.getX());
}

void MotionNotification::PositionGetterY(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    MotionNotification* notification=Unwrap<MotionNotification>(self);
    info.GetReturnValue().Set(notification->getState().Position.getY());
}

void MotionNotification::SpeedGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    MotionNotification* notification=Unwrap<MotionNotification>(self);
    info.GetReturnValue().Set(notification->getState().Speed);
}

void MotionNotification::StateGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    MotionNotification* notification=Unwrap<MotionNotification>(self);
    info.GetReturnValue().Set(notification->getState().State);
}

}
