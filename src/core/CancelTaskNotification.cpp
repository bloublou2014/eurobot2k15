#include "CancelTaskNotification.h"

namespace robot{

string CancelTaskNotification::NAME="CancelTask";
void CancelTaskNotification::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate);
    tpl->SetClassName(String::NewFromUtf8(isolate, CancelTaskNotification::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(CancelTaskNotification::NAME, tpl->GetFunction());
    provider->setObjectTemplate(CancelTaskNotification::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, CancelTaskNotification::NAME.c_str()), tpl->GetFunction());
}

}
