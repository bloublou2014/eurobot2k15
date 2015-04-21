#include "MotionCommand.h"

namespace motion {

string MotionCommand::NAME="MotionCommand";


string MoveForward::NAME="MoveForward";
void MoveForward::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, MoveForward::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(MoveForward::NAME, tpl->GetFunction());
    provider->setObjectTemplate(MoveForward::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, MoveForward::NAME.c_str()), tpl->GetFunction());
}

void MoveForward::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        int value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        MoveForward* obj = new MoveForward(value);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

string RotateFor::NAME="RotateFor";
void RotateFor::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, RotateFor::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(RotateFor::NAME, tpl->GetFunction());
    provider->setObjectTemplate(RotateFor::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, RotateFor::NAME.c_str()), tpl->GetFunction());
}

void RotateFor::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        int value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        RotateFor* obj = new RotateFor(value);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

// tome edit

string RotateTo::NAME="RotateTo";
void RotateTo::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, RotateTo::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(RotateTo::NAME, tpl->GetFunction());
    provider->setObjectTemplate(RotateTo::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, RotateTo::NAME.c_str()), tpl->GetFunction());
}

void RotateTo::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        int value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        RotateTo* obj = new RotateTo(value);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

// 2

string SetSpeedMotion::NAME="SetSpeedMotion";
void SetSpeedMotion::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, SetSpeedMotion::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(SetSpeedMotion::NAME, tpl->GetFunction());
    provider->setObjectTemplate(SetSpeedMotion::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, SetSpeedMotion::NAME.c_str()), tpl->GetFunction());
}

void SetSpeedMotion::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        int value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        SetSpeedMotion* obj = new SetSpeedMotion(value);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

// 3

string StopMovement::NAME="StopMovement";
void StopMovement::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, StopMovement::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(StopMovement::NAME, tpl->GetFunction());
    provider->setObjectTemplate(StopMovement::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, StopMovement::NAME.c_str()), tpl->GetFunction());
}

void StopMovement::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        StopMovement* obj = new StopMovement();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}
// 4

string MoveToPosition::NAME="MoveToPosition";
void MoveToPosition::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, MoveToPosition::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(MoveToPosition::NAME, tpl->GetFunction());
    provider->setObjectTemplate(MoveToPosition::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, MoveToPosition::NAME.c_str()), tpl->GetFunction());
}

void MoveToPosition::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        int valueXcord = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        int valueYcord = args[1]->IsUndefined() ? 0 : args[1]->NumberValue();

        MoveToPosition* obj = new MoveToPosition(Point2D(valueXcord,valueYcord));
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}
// 5

string SetEnemyDetector::NAME="SetEnemyDetector";
void SetEnemyDetector::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, SetEnemyDetector::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(SetEnemyDetector::NAME, tpl->GetFunction());
    provider->setObjectTemplate(SetEnemyDetector::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, SetEnemyDetector::NAME.c_str()), tpl->GetFunction());
}

void SetEnemyDetector::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.Length()<1){
        return isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Not enaugh params"));
    }

    if (args.IsConstructCall()){
        // Invoked as constructor: `new MyObject(...)`
        int type = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        SetEnemyDetector* obj;
        switch (type){
        case 2:
            obj=new SetEnemyDetector(SetEnemyDetector::Type::DETECT_ALL);
            break;
        case 1:
            obj=new SetEnemyDetector(SetEnemyDetector::Type::NO_FIELD_CHECK);
            break;
        case 0:
            obj=new SetEnemyDetector(SetEnemyDetector::Type::DONT_CHECK);
            break;
        default:
            return isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Invalid value"));
            break;
        }

        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

string MoveArc::NAME="MoveArc";
void MoveArc::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, MoveArc::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(MoveArc::NAME, tpl->GetFunction());
    provider->setObjectTemplate(MoveArc::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, MoveArc::NAME.c_str()), tpl->GetFunction());
}

void MoveArc::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        int valueXcord = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        int valueYcord = args[1]->IsUndefined() ? 0 : args[1]->NumberValue();
        int valueAangle = args[2]->IsUndefined() ? 0 : args[2]->NumberValue();

        MoveArc* obj = new MoveArc(Point2D(valueXcord,valueYcord),valueAangle);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

string SetPosition::NAME="SetPosition";
void SetPosition::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, SetPosition::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(SetPosition::NAME, tpl->GetFunction());
    provider->setObjectTemplate(SetPosition::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, SetPosition::NAME.c_str()), tpl->GetFunction());
}

void SetPosition::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        int x = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        int y = args[1]->IsUndefined() ? 0 : args[1]->NumberValue();
        int orientation = args[2]->IsUndefined() ? 0 : args[2]->NumberValue();
        SetPosition* obj = new SetPosition(x,y,orientation);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

string MotionCommandError::NAME="MotionCommandError";
void MotionCommandError::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate);
    tpl->SetClassName(String::NewFromUtf8(isolate, MotionCommandError::NAME.c_str()));
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "errorType"),
                            MotionCommandError::ErrorTypeGetter);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(MotionCommandError::NAME, tpl->GetFunction());
    provider->setObjectTemplate(MotionCommandError::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, MotionCommandError::NAME.c_str()), tpl->GetFunction());
}

void MotionCommandError::ErrorTypeGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    MotionCommandError* notification=Unwrap<MotionCommandError>(self);
    info.GetReturnValue().Set((int)notification->getType());
}


} // end namespace


