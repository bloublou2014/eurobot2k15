#ifndef LIFTNOTIFICATION_H
#define LIFTNOTIFICATION_H

#include "messages/Notification.h"
#include "messages/CommandResponse.h"

using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;
using namespace robot;

namespace executor{

class LIftNotification : public Notification
{
public:
    enum Side{
        RIGHT=1,
        LEFT=2
    };

    static string NAME;
    static void Init(Handle<Object> exports);
    //Getters
    static void SideGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void CountGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);

    LIftNotification(Side _side, int _count):Notification(NAME,"LiftExecutor"),side(_side), count(_count){}
    LIftNotification(const LIftNotification& ln):Notification(ln),side(ln.side), count(ln.count){}
    Side getSide() const{ return side; }
    int getCount() const{ return count; }

    Message* clone(){
        return new LIftNotification(*this);
    }

private:
    int side;
    int count;
};

class LiftProgressNotification : public Notification{
public:
    enum ProgressType{
        SENSOR_TRIGGERED=0,
        ROLLER_CATCHED=1
    };

    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    LiftProgressNotification(ProgressType _type, const string& to):
        Notification(NAME,"LiftExecutor"),progressType(_type) {}
    LiftProgressNotification(const LiftProgressNotification& mcr):Notification(mcr),
        progressType(mcr.progressType){}

    static void TypeGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);

    inline int getType(){ return progressType; }

    Message* clone(){
        return new LiftProgressNotification(*this);
    }
private:
    ProgressType progressType;
};

}
#endif // LIFTNOTIFICATION_H
