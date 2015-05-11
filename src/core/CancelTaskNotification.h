#ifndef CANCELTASKNOTIFICATION_H
#define CANCELTASKNOTIFICATION_H

#include "messages/Notification.h"
#include "utils/javascript/JavaScriptMessageProvider.h"

using javascript::JavaScriptMessageProvider;

namespace robot{

class CancelTaskNotification : public Notification{
public:
    static string NAME;

    static void Init(Handle<Object> exports);
    /* Constructor */
//    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    CancelTaskNotification():Notification(NAME,"Scheduler"){}
    CancelTaskNotification(const CancelTaskNotification& ct):Notification(ct){}

    Message* clone(){
        return new CancelTaskNotification(*this);
    }
};

}
#endif // CANCELTASKNOTIFICATION_H
