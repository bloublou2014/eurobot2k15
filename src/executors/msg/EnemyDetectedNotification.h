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
        FRONT=0,
        BACK=1
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

class BeaconNotification: public Notification{
public:


    static string NAME;
    static void Init(Handle<Object> exports);
    //Getters
    static void MaliValidDataGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void MaliCordXGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void MaliCordYGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void VelikiValidDataGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void VelikiCordXGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void VelikiCordYGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);

    BeaconNotification(bool _maliValidData, bool _velikiValidData, short _maliCordX, short _maliCordY, short _velikiCordX, short _velikiCordY ):Notification(NAME,"BeaconNotification"),
        maliCordX(_maliCordX), maliCordY(_maliCordY), velikiCordX(_velikiCordX), velikiCordY(_velikiCordY),
        maliValidData(_maliValidData), velikiValidData(_velikiValidData){}

    BeaconNotification(const BeaconNotification& bn):Notification(bn),
       maliCordX(bn.maliCordX),maliCordY(bn.maliCordY),velikiCordX(bn.velikiCordX),velikiCordY(bn.velikiCordY),
       maliValidData(bn.maliValidData), velikiValidData(bn.velikiValidData){}

    short getMaliCordX(){return maliCordX;}
    short getMaliCordY(){return maliCordY;}
    short getVelikiCordX(){return velikiCordX;}
    short getVelikiCordY(){return velikiCordY;}
    bool  getMaliValidData(){ return maliValidData;}
    bool  getVelikiValidData(){ return velikiValidData;}

    Message* clone(){
        return new BeaconNotification(*this);
    }

private:
    bool maliValidData;
    bool velikiValidData;
    short maliCordX;
    short maliCordY;
    short velikiCordX;
    short velikiCordY;
};

}
#endif // ENEMYDETECTEDNOTIFICATION_H
