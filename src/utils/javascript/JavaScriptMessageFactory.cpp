#include "JavaScriptMessageFactory.h"

namespace javascript{

void JavaScriptMessageFactory::init(Handle<Object> exportObject){
    CountdownCommand::Init(exportObject);
}

Handle<Function> JavaScriptMessageFactory::getObjectConstructor(const string& name){
    Isolate* isolate=Isolate::GetCurrent();

    EscapableHandleScope scope(isolate);
    Local<Function> result= Local<Function>::New(isolate, registeredConstructors[name]);
    return scope.Escape(result);
}

void JavaScriptMessageFactory::setObjectConstructor(const string& name, Handle<Function> newTemplate){
    Isolate* isolate=Isolate::GetCurrent();
    registeredConstructors[name].Reset(isolate, newTemplate);
}

}
