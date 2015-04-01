#include "JavaScriptMessageFactory.h"

namespace javascript{

void JavaScriptMessageFactory::init(Handle<Object> exportObject){
    //Dummy test commands
    robot::CountdownCommand::Init(exportObject);
    robot::TimePassedNotification::Init(exportObject);
    robot::CommandResponse::Init(exportObject);
    //Motion commands
    motion::MoveForward::Init(exportObject);
    motion::RotateFor::Init(exportObject);
    motion::RotateTo::Init(exportObject);
    motion::MoveToPosition::Init(exportObject);
    motion::MoveArc::Init(exportObject);
    motion::StopMovement::Init(exportObject);
    motion::SetSpeedMotion::Init(exportObject);
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

Handle<Object> JavaScriptMessageFactory::wrapObject(const string& name, Isolate* isolate, robot::Message* ptr){
    EscapableHandleScope scope(isolate);

    if (registeredObjectTemplates.find(name)!=registeredObjectTemplates.end()){
        Local<ObjectTemplate> tmpl= Local<ObjectTemplate>::New(isolate, registeredObjectTemplates[name]);
        Local<Object> result=tmpl->NewInstance();
        ObjectWrap* oldObject=ObjectWrap::Unwrap<ObjectWrap>(result);
        ObjectWrap* tp=static_cast<ObjectWrap*>(ptr);
        tp->Wrap(result, isolate);
        oldObject->deleteRef();
        delete oldObject;
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
