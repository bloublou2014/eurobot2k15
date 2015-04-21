#include "SideNotification.h"


using namespace robot;

namespace executor{

string SideNotification::NAME="SideNotification";
void SideNotification::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate);
    tpl->SetClassName(String::NewFromUtf8(isolate, SideNotification::NAME.c_str()));
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "side"),
                            SideGetter);

    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(SideNotification::NAME, tpl->GetFunction());
    provider->setObjectTemplate(SideNotification::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, SideNotification::NAME.c_str()), tpl->GetFunction());
}

void SideNotification::SideGetter(Local<String> property, const PropertyCallbackInfo<Value>& info){
    Local<Object> self = info.Holder();
    SideNotification* notification=Unwrap<SideNotification>(self);
    info.GetReturnValue().Set(notification->getSide());
}


} // end notification
