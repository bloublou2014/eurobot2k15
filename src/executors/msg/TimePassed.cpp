#include "TimePassed.h"

namespace robot{

int TimePassedNotification::getPassedTime() const{
    return passedTime;
}

void TimePassedNotification::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "TimePassedNotification"));
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "passedTime"),
                            ValueGetter, ValueSetter);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor("TimePassedNotification",tpl->GetFunction());
    provider->setObjectTemplate("TimePassedNotification", tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, "TimePassedNotification"), tpl->GetFunction());
}

void TimePassedNotification::ValueGetter(Local<String> property, const PropertyCallbackInfo<Value>& info) {
    Local<Object> self = info.Holder();
    TimePassedNotification* command=Unwrap<TimePassedNotification>(self);
    int value = command->passedTime;
    info.GetReturnValue().Set(value);
}

void TimePassedNotification::ValueSetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
    Local<Object> self = info.Holder();
    TimePassedNotification* command=Unwrap<TimePassedNotification>(self);
    command->passedTime=value->Int32Value();
}

void TimePassedNotification::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()){
        // Invoked as constructor: `new MyObject(...)`
        int value = args[0]->IsUndefined() ? 42 : args[0]->NumberValue();
        TimePassedNotification* obj = new TimePassedNotification("ASD",value);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
        Handle<Function> constructor= provider->getObjectConstructor("TimePassedNotification");
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        args.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

}
