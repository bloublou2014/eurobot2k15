#include "JavaScriptMessageFactory.h"

namespace javascript{

void JavaScriptMessageFactory::init(Handle<Object> exportObject){
    robot::CountdownCommand::Init(exportObject);
    robot::TimePassedNotification::Init(exportObject);
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

Handle<Object> JavaScriptMessageFactory::wrapObject(const string& name, Isolate* isolate, void* ptr){
    EscapableHandleScope scope(isolate);

    if (registeredObjectTemplates.find(name)!=registeredObjectTemplates.end()){
        //TODO: Fix, not working!
        Local<ObjectTemplate> tmpl= Local<ObjectTemplate>::New(isolate, registeredObjectTemplates[name]);
        Local<Object> result=tmpl->NewInstance();
        ObjectWrap* wrap=static_cast<ObjectWrap*>(ptr);
        result->SetAlignedPointerInInternalField(0, ptr);
        return scope.Escape(result);
    }
    Local<Object> empty;
    return scope.Escape(empty);
}

void JavaScriptMessageFactory::setObjectTemplate(const string& name, Handle<ObjectTemplate> tmpl){
    Isolate* isolate=Isolate::GetCurrent();
    registeredObjectTemplates[name].Reset(isolate, tmpl);
}

}
