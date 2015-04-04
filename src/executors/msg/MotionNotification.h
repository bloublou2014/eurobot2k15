#ifndef _MOTIONNOTIFICATION_H
#define _MOTIONNOTIFICATION_H

#include "messages/Notification.h"
#include "utils/helper/MotionState.h"

using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;
using namespace robot;

namespace motion{

class MotionNotification : public Notification{
public:
    static string NAME;

    static void Init(Handle<Object> exports);
    /* Constructor */
//    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void DirectionGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void OrientationGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void PositionGetterX(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void PositionGetterY(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void SpeedGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void StateGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);

    MotionNotification(MotionState _state=MotionState()):Notification(NAME,"MotionExecutor"),state(_state){}
    MotionNotification(const MotionNotification& mn):Notification(mn),state(mn.state){}
    MotionState getState() const{ return state; }

    Message* clone(){
        return new MotionNotification(*this);
    }
private:
    MotionState state;
};

}

#endif //_MOTIONNOTIFICATION_H
