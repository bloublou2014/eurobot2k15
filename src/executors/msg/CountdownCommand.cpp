#include "CountdownCommand.h"

namespace robot {

void CountdownCommand::setCountdownValue(int _value){
    value=_value;
}

int CountdownCommand::getCountdownValue() const {
    return value;
}

/*Create template for object*/
Handle<ObjectTemplate> CountdownCommand::CreateTemplate(Isolate* isolate){
    EscapableHandleScope scope(isolate);

    Local<ObjectTemplate> instanceTemplate=ObjectTemplate::New(isolate);
    instanceTemplate->SetInternalFieldCount(1);

    instanceTemplate->SetAccessor(String::NewFromUtf8(isolate, "value"),
                        ValueGetter, ValueSetter);

    return scope.Escape(instanceTemplate);
}

void CountdownCommand::ValueGetter(Local<String> property, const PropertyCallbackInfo<Value>& info) {
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    int value = static_cast<CountdownCommand*>(ptr)->value;
    info.GetReturnValue().Set(value);
}

void CountdownCommand::ValueSetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<CountdownCommand*>(ptr)->value = value->Int32Value();
}

Handle<FunctionTemplate> CountdownCommand::CreateConstructorTemplate(Isolate* isolate){
    EscapableHandleScope scope(isolate);

    Local<FunctionTemplate> funTemplate=FunctionTemplate::New(isolate,constructorCall);

    return scope.Escape(funTemplate);
}

void CountdownCommand::constructorCall(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate=args.GetIsolate();
    HandleScope scope(isolate);
    if (!args.IsConstructCall()){
        args.GetIsolate()->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
        return;
    }
    if (args.Length()<1){
        args.GetIsolate()->ThrowException(v8::String::NewFromUtf8(isolate, "Constructor takes one parameter."));
        return;
    }

    JavaScriptMessageProvider* messageProvider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    JavaScriptMessage* msg=messageProvider->getMessageHandler("CountdownCommand");

    int value=args[0]->NumberValue();

    Local<ObjectTemplate> instanceTemplate=msg->getObjectTemplate();

    Local<Object> instance=instanceTemplate->NewInstance();
    Handle<External> refferencePtr=External::New(isolate, new CountdownCommand(value));
    instance->SetInternalField(0, refferencePtr);

    args.GetReturnValue().Set(instance);
}

}
