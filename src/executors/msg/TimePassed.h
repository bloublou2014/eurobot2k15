#ifndef _TIMEPASSED_H
#define _TIMEPASSED_H

#include "messages/Notification.h"
#include "utils/javascript/JavaScriptMessageProvider.h"
#include "utils/javascript/ObjectWrap.h"

using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;

namespace robot{

class TimePassedNotification : public Notification{
public:
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    /* Getters and setters */
    static void ValueGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void ValueSetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info);

    TimePassedNotification(const string& sender,int time):Notification("TimePassedNotification",sender),passedTime(time){}
    TimePassedNotification(const TimePassedNotification& tpn):Notification(tpn),passedTime(tpn.passedTime){}
    int getPassedTime() const;

    Message* clone(){
        return new TimePassedNotification(*this);
    }
protected:
private:
    int passedTime;
};

}

#endif //_TIMEPASSED_H
