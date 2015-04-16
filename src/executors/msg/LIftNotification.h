#ifndef LIFTNOTIFICATION_H
#define LIFTNOTIFICATION_H

#include "messages/Notification.h"

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

}
#endif // LIFTNOTIFICATION_H
