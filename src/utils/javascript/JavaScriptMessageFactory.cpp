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

Handle<Object> JavaScriptMessageFactory::wrapObject(const string& name, Isolate* isolate, robot::Message* ptr){
    EscapableHandleScope scope(isolate);

    if (registeredObjectTemplates.find(name)!=registeredObjectTemplates.end()){
        //TODO: Fix, not working!
        Local<ObjectTemplate> tmpl= Local<ObjectTemplate>::New(isolate, registeredObjectTemplates[name]);
        Local<Object> result=tmpl->NewInstance();
        ObjectWrap* oldObject=ObjectWrap::Unwrap<ObjectWrap>(result);
        ObjectWrap* tp=static_cast<ObjectWrap*>(ptr);
        tp->Wrap(result, isolate);
//        TimePassedNotification* tp=static_cast<TimePassedNotification*>(result->GetAlignedPointerFromInternalField(0));
//        TimePassedNotification* tp=ObjectWrap::Unwrap<TimePassedNotification>(result);
//        int number=tp->getPassedTime();
//        tp->passedTime=18;
//        ObjectWrap* wrap=static_cast<ObjectWrap*>(ptr);
//        wrap->Wrap(result,isolate);
//        result->SetAlignedPointerInInternalField(0, ptr);
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
