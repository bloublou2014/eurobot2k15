#include "CountdownCommand.h"

namespace robot {

void CountdownCommand::setCountdownValue(int _value){
    value=_value;
}

int CountdownCommand::getCountdownValue() const {
    return value;
}

string CountdownCommand::getName() const{
    return "CountdownCommand";
}

void CountdownCommand::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "CountdownCommand"));
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "value"),
                            ValueGetter, ValueSetter);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor("CountdownCommand",tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "CountdownCommand"), tpl->GetFunction());
}

void CountdownCommand::ValueGetter(Local<String> property, const PropertyCallbackInfo<Value>& info) {
    Local<Object> self = info.Holder();
    CountdownCommand* command=Unwrap<CountdownCommand>(self);
    int value = command->value;
    info.GetReturnValue().Set(value);
}

void CountdownCommand::ValueSetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
    Local<Object> self = info.Holder();
    CountdownCommand* command=Unwrap<CountdownCommand>(self);
    command->value=value->Int32Value();
}

void CountdownCommand::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        int value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        CountdownCommand* obj = new CountdownCommand(value);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

}
