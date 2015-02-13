#include "CommandResponse.h"

namespace robot {

char* CommandResponse::NAME="CommandResponse";

static void CommandResponse::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, NAME));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(NAME,tpl->GetFunction());
    provider->setObjectTemplate(NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, NAME), tpl->GetFunction());
}

static void CommandResponse::New(const v8::FunctionCallbackInfo<v8::Value>& args){
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()){
        CommandResponse* obj = new CommandResponse(NAME,"","");
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        const int argc = 0;
        Local<Value> argv[argc] = { };
        JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
        Handle<Function> constructor= provider->getObjectConstructor(NAME);
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        args.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

string CommandResponse::getDestination() const{
    return destination;
}

ResponseStatus CommandResponse::getStatus() const{
    return status;
}

void CommandResponse::setId(int _id){
    id=_id;
}

int CommandResponse::getId() const{
    return id;
}

string CommandResponse::getName() const{
    return name;
}

}
