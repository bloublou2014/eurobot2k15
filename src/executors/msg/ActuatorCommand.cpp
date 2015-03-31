#include "ActuatorCommand.h"

namespace executor {

string ActuatorCommand::NAME="ActuatorCommand";



/*
ActuatorTypePackage::ActuatorTypePackage(){
    ExecutorName[LIFT_CENTER] = "LiftCenterExecutor";
    ExecutorName[LIFT_LEFT] = "LiftLeftExecutor";
    ExecutorName[LIFT_RIGHT] = "LiftRightExecutor";
    ExecutorName[POPCORN] = "PopcornExecutor";
    ExecutorName[BALLS] = "BallsExecutor";
 */

Command* ActuatorAction::LiftLeft(ActuatorType _type){
    switch (_type){
    case GET_OBJECT :    return (Command*) new GetObject(LIFT_LEFT);
    case UNLOAD_OBJECT : return (Command*) new UnloadObject(LIFT_LEFT);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(LIFT_LEFT);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(LIFT_LEFT);
    default : return NULL;
    }
}

Command* ActuatorAction::LiftRight(ActuatorType _type){
    switch (_type){
    case GET_OBJECT :    return (Command*) new GetObject(LIFT_RIGHT);
    case UNLOAD_OBJECT : return (Command*) new UnloadObject(LIFT_RIGHT);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(LIFT_RIGHT);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(LIFT_RIGHT);
    default : return NULL;
    }
}

Command* ActuatorAction::LiftCenter(ActuatorType _type){
    switch (_type){
    case GET_OBJECT :    return (Command*) new GetObject(LIFT_CENTER);
    case UNLOAD_OBJECT : return (Command*) new UnloadObject(LIFT_CENTER);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(LIFT_CENTER);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(LIFT_CENTER);
    default : return NULL;
    }
}

Command* ActuatorAction::Flap(ActuatorType _type){
    switch (_type){
    case KICK_RIGHT :    return (Command*) new KickRight(FLAP);
    case KICK_LEFT :   return (Command*) new KickLeft(FLAP);
    case UNKICK_RIGHT : return (Command*) new UnKickRight(FLAP);
    case UNKICK_LEFT  :  return (Command*) new UnKickLeft(FLAP);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(FLAP);
    default : return NULL;
    }
}
Command* ActuatorAction::Popcorn(ActuatorType _type){
    switch (_type){
    case UNLOAD_POPCORN :    return (Command*) new UnloadPopcorn(POPCORN);
    case GET_POPCORN : return (Command*) new GetPopcorn(POPCORN);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(POPCORN);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(POPCORN);
    default : return NULL;
    }
}

Command* ActuatorConfig::LiftLeft(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(LIFT_LEFT, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(LIFT_LEFT, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::LiftRight(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(LIFT_RIGHT, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(LIFT_RIGHT, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::LiftCenter(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(LIFT_CENTER, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(LIFT_CENTER, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::Popcorn(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(POPCORN, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(POPCORN, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::Flap(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(FLAP, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(FLAP, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

}
