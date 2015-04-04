#include "GetMotionState.h"

namespace motion {

//GetMotionState
string GetMotionState::NAME="GetMotionState";
void GetMotionState::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, GetMotionState::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(GetMotionState::NAME, tpl->GetFunction());
    provider->setObjectTemplate(GetMotionState::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, GetMotionState::NAME.c_str()), tpl->GetFunction());
}

void GetMotionState::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        GetMotionState* obj = new GetMotionState();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

//GetMotionStateResponse
string GetMotionStateResponse::NAME="GetMotionStateResponse";
void GetMotionStateResponse::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate);
    tpl->SetClassName(String::NewFromUtf8(isolate, GetMotionStateResponse::NAME.c_str()));
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
    provider->setObjectConstructor(GetMotionStateResponse::NAME, tpl->GetFunction());
    provider->setObjectTemplate(GetMotionStateResponse::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, GetMotionStateResponse::NAME.c_str()), tpl->GetFunction());
}

void GetMotionStateResponse::DirectionGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    GetMotionStateResponse* notification=Unwrap<GetMotionStateResponse>(self);
    info.GetReturnValue().Set(notification->getState().Direction);
}

void GetMotionStateResponse::OrientationGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    GetMotionStateResponse* notification=Unwrap<GetMotionStateResponse>(self);
    info.GetReturnValue().Set(notification->getState().Orientation);
}

void GetMotionStateResponse::PositionGetterX(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    GetMotionStateResponse* notification=Unwrap<GetMotionStateResponse>(self);
    info.GetReturnValue().Set(notification->getState().Position.getX());
}

void GetMotionStateResponse::PositionGetterY(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    GetMotionStateResponse* notification=Unwrap<GetMotionStateResponse>(self);
    info.GetReturnValue().Set(notification->getState().Position.getY());
}

void GetMotionStateResponse::SpeedGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    GetMotionStateResponse* notification=Unwrap<GetMotionStateResponse>(self);
    info.GetReturnValue().Set(notification->getState().Speed);
}

void GetMotionStateResponse::StateGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    GetMotionStateResponse* notification=Unwrap<GetMotionStateResponse>(self);
    info.GetReturnValue().Set(notification->getState().State);
}

}
