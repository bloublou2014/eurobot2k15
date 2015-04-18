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
        BRKON,
        BEACON,
        SENSOR
    };

    static string NAME;
    static void Init(Handle<Object> exports);
    //Getters
    static void TypeGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void AngleGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void DetectedGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);

    EnemyDetectedNotification(Type _type, int _angle, bool _detected=true):Notification(NAME,"EnemyDetector"),
        type(_type), angle(_angle), detected(_detected){}
    EnemyDetectedNotification(const EnemyDetectedNotification& ed):Notification(ed),
        type(ed.type), angle(ed.angle), detected(ed.detected){}
    Type getType() const{ return type; }
    int getAngle() const{ return angle; }
    bool isDetected() const{ return detected; }

    Message* clone(){
        return new EnemyDetectedNotification(*this);
    }
private:
    Type type;
    int angle;
    bool detected;
};

}
#endif // ENEMYDETECTEDNOTIFICATION_H
