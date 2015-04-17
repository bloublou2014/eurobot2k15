#ifndef ENEMYDETECTEDNOTIFICATION_H
#define ENEMYDETECTEDNOTIFICATION_H

#include "messages/Notification.h"

using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;
using namespace robot;

namespace executor{

class EnemyDetectedNotification : public Notification
{
public:
    enum Type{
        BRKSON,
        BEACON,
        SENSOR
    };

    static string NAME;
    static void Init(Handle<Object> exports);
    //Getters
    static void TypeGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void AngleGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);

    EnemyDetectedNotification(Type _type, int _angle):Notification(NAME,"EnemyDetector"),type(_type), angle(_angle){}
    EnemyDetectedNotification(const EnemyDetectedNotification& ed):Notification(ed),type(ed.type), angle(ed.angle){}
    Type getType() const{ return type; }
    int getAngle() const{ return angle; }

    Message* clone(){
        return new EnemyDetectedNotification(*this);
    }
private:
    Type type;
    int angle;
};

}
#endif // ENEMYDETECTEDNOTIFICATION_H
