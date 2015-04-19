#ifndef ABSTRACTACTUATORCOMMAND_H
#define ABSTRACTACTUATORCOMMAND_H

#include <boost/assign.hpp>

#include "messages/Command.h"
#include "messages/CommandResponse.h"

#include "drivers/actuators/Servos.h"

//#include "utils/javascript/ObjectWrap.h"

using namespace boost::assign;
using namespace robot;
using namespace std;
using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;

namespace executor{

enum Executors{LIFT_LEFT,
               LIFT_RIGHT,
               LIFT_CENTER,
               POPCORN,
               FLAP,
               MOTION,
               ENEMY_DETECT,
               CARPET,
               NULL_EXECUTOR
              };

enum ServoType{ LIFT_SRVO,
                DOOR_SERVO,
                GRAB_SERVO,
                POPCORN_SERVO,
                FLAP_SERVO
                //TODO
              };


enum ActuatorType{SET_START_CONFIG,
                  GET_OBJECT,
                  UNLOAD_OBJECT,
                  KICK_RIGHT,
                  KICK_LEFT,
                  UNKICK_RIGHT,
                  UNKICK_LEFT,
                  GET_POPCORN,
                  UNLOAD_POPCORN,
                  SET_SPEED,
                  SET_POSITION,
                  RELOAD_CONFIG,
                  GET_OBJECT_STOP,
                  START_BRXON,
                  STOP_BRXON,
                  START_BEACON,
                  STOP_BEACON,
                  LEAVE_CARPET,
                  NULL_ACTION,
                  CALLBACK_GET,
                 };

typedef map<Executors, string> ExecutorsMap;
extern ExecutorsMap ExecutorName;

class ActuatorCommand:public Command{
public:
    static string NAME;

    ActuatorCommand(): Command(ActuatorCommand::NAME, "tome"){}
    ActuatorCommand(ActuatorType _type, Executors _executor):Command(ActuatorCommand::NAME, (ExecutorName[_executor]) ),type(_type),executors(_executor){}
    ActuatorCommand(const ActuatorCommand& obj):Command(/*obj*/ ActuatorCommand::NAME, (ExecutorName[obj.executors]) ),type(obj.type), executors(obj.executors){}

    Message* clone(){
        return new ActuatorCommand(*this);
    }

    ActuatorType getActuatorType() const {return type;}
private:
    ActuatorType type;
    Executors executors;
};

class SetStartConfig: public ActuatorCommand{
public:

    SetStartConfig(Executors _executor): ActuatorCommand(SET_START_CONFIG,_executor){}

};

class GetObject: public ActuatorCommand{
public:
    GetObject(Executors _executor): ActuatorCommand(GET_OBJECT,_executor){}
};

class GetObjectStop: public ActuatorCommand{
public:
    GetObjectStop(Executors _executor): ActuatorCommand(GET_OBJECT_STOP, _executor){}
};

class UnloadObject: public ActuatorCommand{
public:
    UnloadObject(Executors _executor): ActuatorCommand(UNLOAD_OBJECT, _executor){}
};

class KickRight: public ActuatorCommand{
public:
    KickRight(Executors _executor): ActuatorCommand(KICK_RIGHT, _executor){}
};

class KickLeft: public ActuatorCommand{
public:
    KickLeft(Executors _executor): ActuatorCommand(KICK_LEFT, _executor){}
};

class UnKickRight: public ActuatorCommand{
public:
    UnKickRight(Executors _executor): ActuatorCommand(UNKICK_RIGHT, _executor){}
};

class UnKickLeft: public ActuatorCommand{
public:
    UnKickLeft(Executors _executor): ActuatorCommand(UNKICK_LEFT, _executor){}
};

class GetPopcorn: public ActuatorCommand{
public:
    GetPopcorn(Executors _executor): ActuatorCommand(GET_POPCORN, _executor){}
};

class UnloadPopcorn: public ActuatorCommand{
public:
    UnloadPopcorn(Executors _executor): ActuatorCommand(UNLOAD_POPCORN,_executor){}
};

class ReloadConfig: public ActuatorCommand{
public:
    ReloadConfig(Executors _executor): ActuatorCommand(RELOAD_CONFIG, _executor){}
};

class StartBrxon: public ActuatorCommand{
public:
    StartBrxon(Executors _executor):ActuatorCommand(START_BRXON, _executor){}
};

class StopBrxon: public ActuatorCommand{
public:
    StopBrxon(Executors _executor):ActuatorCommand(STOP_BRXON, _executor){}
};

class StartBeacon: public ActuatorCommand{
public:
    StartBeacon(Executors _executor):ActuatorCommand(START_BEACON, _executor){}
};

class StopBeacon: public ActuatorCommand{
public:
    StopBeacon(Executors _executor):ActuatorCommand(STOP_BEACON, _executor){}
};

class LeaveCarpet: public ActuatorCommand{
public:
    LeaveCarpet(Executors _executor):ActuatorCommand(LEAVE_CARPET, _executor){}
};

class CallbackGet: public ActuatorCommand{
public:
    CallbackGet(Executors _executor):ActuatorCommand(CALLBACK_GET, _executor){}
};

class SetSpeed: public ActuatorCommand {
public:
    SetSpeed(Executors _executor, ServoType _servo,  int _value): ActuatorCommand(SET_SPEED, _executor), value(_value){}
    SetSpeed(const SetSpeed& obj):ActuatorCommand(obj), value(obj.value), servo(obj.servo){}
    int getSpeed(){return value;}
private:
    int value;
    ServoType servo;
};

class SetPosition: public ActuatorCommand {
public:
    SetPosition(Executors _executor, ServoType _servo,  int _value): ActuatorCommand(SET_POSITION, _executor), value(_value), servo(_servo){}
    SetPosition(const SetPosition& obj):ActuatorCommand(obj),value(obj.value),servo(obj.servo){}
    int getPosition(){return value;}
private:
    int value;
    ServoType servo;
};


class ActuatorAction: public ActuatorCommand {

public:
    static Command* LiftLeft(ActuatorType _type);
    static Command* LiftRight(ActuatorType _type);
    static Command* LiftCenter(ActuatorType _type);
    static Command* Popcorn(ActuatorType _type);
    static Command* Flap(ActuatorType _type);
    static Command* EnemyDetector(ActuatorType _type);
    static Command* Carpet(ActuatorType _type);
private:

};

class ActuatorConfig: public ActuatorCommand {
public:
    static Command* LiftLeft(ActuatorType _actuator, ServoType _servo, int _value);
    static Command* LiftRight(ActuatorType _actuator, ServoType _servo, int _value);
    static Command* LiftCenter(ActuatorType _actuator, ServoType _servo, int _value);
    static Command* Popcorn(ActuatorType _actuator, ServoType _servo, int _value);
    static Command* Flap(ActuatorType _actuator, ServoType _servo, int _value);
};

class ActuatorCommandJS { // PROGRESS
public:
    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    ActuatorCommandJS(string _executor, string _action): executorString(_executor), actionString(_action){}
    ActuatorCommandJS(const ActuatorCommandJS &acJS): executorString(acJS.executorString), actionString(acJS.actionString){}

    //Message* clone(){
    //    return new ActuatorCommandJS(*this);
    //}
private:

    //Command* createCommand( Executors _executor, ActuatorType _actuator );
    //void parse(string _executorString, string _actuatorString, Executors *_executor, ActuatorType _action ); // TODO
    static Command* parseCreateCommand(string _executorString, string _actionString, bool *_success);

    string executorString;
    string actionString;
    Executors executor;
    ActuatorType action;
};

}// end namespace


#endif // ABSTRACTACTUATORCOMMAND_H
