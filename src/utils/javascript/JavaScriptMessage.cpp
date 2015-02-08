#include "JavaScriptMessage.h"

namespace javascript{

JavaScriptMessage::JavaScriptMessage(Isolate* _isolate, ObjectTemplateBuilder objectBuilder, ConstructorTemplateBuilder constructorBuilder):
            objectTemplateBuilder(objectBuilder), constructorTemplateBuilder(constructorBuilder), isolate(_isolate){

    HandleScope scope(isolate);

    Handle<FunctionTemplate> newFunctionTemplate=constructorBuilder(isolate);
    constructorTemplate.Reset(isolate, newFunctionTemplate);

    Handle<ObjectTemplate> newObjectTemplate=objectBuilder(isolate);
    objectTemplate.Reset(isolate, newObjectTemplate);
}

ObjectTemplateBuilder JavaScriptMessage::getObjectBuilder(){
    return objectTemplateBuilder;
}

ConstructorTemplateBuilder JavaScriptMessage::getConstructorBuilder(){
    return constructorTemplateBuilder;
}

Handle<FunctionTemplate> JavaScriptMessage::getConstructorTemplate(){
    EscapableHandleScope scope(isolate);

    Local<FunctionTemplate> result= Local<FunctionTemplate>::New(isolate, constructorTemplate);

    return scope.Escape(result);
}

Handle<ObjectTemplate> JavaScriptMessage::getObjectTemplate(){
    EscapableHandleScope scope(isolate);

    Local<ObjectTemplate> result= Local<ObjectTemplate>::New(isolate, objectTemplate);

    return scope.Escape(result);
}

Handle<Function> JavaScriptMessage::createConstructorFunctoin(Isolate* isolate){
    EscapableHandleScope scope(isolate);

    Handle<FunctionTemplate> funTemple= Local<FunctionTemplate>::New(isolate, constructorTemplate);
    Local<Function> result=funTemple->GetFunction();

    return scope.Escape(result);
}


}
