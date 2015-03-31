#include "AbstractLiftExecutor.h"

namespace executor {

void AbstractLiftExecutor::maping(){
    debug("default maping");
    //actuatorHandles[ActuatorCommand::ActuatorType::GET_OBJECT]=static_cast<ActuatorCommandHandle>(&AbstractLiftExecutor::getObject);
    //actuatorHandles[ActuatorCommand::ActuatorType::UNLOAD_OBJECT]=static_cast<ActuatorCommandHandle>(&AbstractLiftExecutor::unloadObject);
    //actuatorHandles[ActuatorCommand::ActuatorType::KICK]=static_cast<ActuatorCommandHandle>(&AbstractLiftExecutor::kick);
    //actuatorHandles[ActuatorCommand::ActuatorType::GET_POPCORN]=static_cast<ActuatorCommandHandle>(&AbstractLiftExecutor::getPopcorn);
    //actuatorHandles[ActuatorCommand::ActuatorType::SET_POSITION]=static_cast<ActuatorCommandHandle>(&AbstractLiftExecutor::setPosition);
    //actuatorHandles[ActuatorCommand::ActuatorType::SET_SPEED]=static_cast<ActuatorCommandHandle>(&AbstractLiftExecutor::setSpeed);
    //actuatorHandles[ActuatorCommand::ActuatorType::RELOAD_CONFIG]=static_cast<ActuatorCommandHandle>(&AbstractLiftExecutor::reloadConfig);

}

void AbstractLiftExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&AbstractLiftExecutor::processActuatorCommand));
}
/*
void AbstractLiftExecutor::getObject(ActuatorCommand * _command){
    bool success;
    GetObject* command = (GetObject*) _command;
    currentActuatorCommand = command;
    success =  GetObjectFunction();
    if (success) {
        currentActuatorCommand = NULL;
    }
}

void AbstractLiftExecutor::unloadObject(ActuatorCommand* _command){
    bool success;
    UnloadObject *command  = (UnloadObject*) _command;
    currentActuatorCommand = command;
    success = UnloadObjectFunction();
    if (success){
        currentActuatorCommand = NULL;
    }
}
*/
/*
void AbstractLiftExecutor::setSpeed(ActuatorCommand *_command){
    bool success = false ;
    SetSpeed* command = (SetSpeed*) _command;
    currentActuatorCommand = command;
    success = SetSpeedFunction();
    if (success){
        currentActuatorCommand = NULL;
    }
}

void AbstractLiftExecutor::setPosition(ActuatorCommand *_command){
    bool success = false;
    SetPosition* command  = (SetPosition*) _command;
    currentActuatorCommand = command;
    if (success){
        currentActuatorCommand = NULL;
    }
}


void AbstractLiftExecutor::reloadConfig(ActuatorCommand* _command){
    bool success = false ;
    ReloadConfig* command = (ReloadConfig*) _command;
    currentActuatorCommand = command;
    debug("reload config with exec name: ");
    success = reload(&value, executorName);
    if (success){
        currentActuatorCommand = NULL;
    }

}

*/

bool AbstractLiftExecutor::GetObjectFunction(){
    debug("GetObject Default function");
    debug("TODO");
    return false;
}

bool AbstractLiftExecutor::UnloadObjectFunction(){
    debug("unloadObject Default function");
    debug("TODO");
    return false;
}

bool AbstractLiftExecutor::SetSpeedFunction(){
    //TODO
    debug("SetSpeed Defalut function");
    debug("TODO");

    return false;
}

bool AbstractLiftExecutor::SetPositionFunction(){
    //TODO
    debug("SetPosition Default function");
    debug("TODO");

    return false;
}

void AbstractLiftExecutor::brodcastNotification(){
    // overide this function
}


void AbstractLiftExecutor::SetHandAddresses(unsigned char _slave_address, unsigned short _position_address, unsigned short _speed_address){
    hand.setServoSlaveAddress(_slave_address);
    hand.setServoPositionAddress(_position_address);
    hand.setServoSpeedAddress(_speed_address);
}

void AbstractLiftExecutor::SetDoorAddresses(unsigned char _slave_address, unsigned short _position_address, unsigned short _speed_address){
    door.setServoSlaveAddress(_slave_address);
    door.setServoPositionAddress(_position_address);
    door.setServoSpeedAddress(_speed_address);
}
void AbstractLiftExecutor::SetLiftAddresses(unsigned char _slave_address, unsigned short _position_address, unsigned short _speed_address){
    lift.setServoSlaveAddress(_slave_address);
    lift.setServoPositionAddress(_position_address);
    lift.setServoSpeedAddress(_speed_address);
}

void AbstractLiftExecutor::SetSensorAddresses(unsigned char _slave_address, unsigned short _position_address){
    sensor.setSlaveAddress(_slave_address);
    sensor.setScanAddress(_position_address);
}

bool AbstractLiftExecutor::doorF(DoorFunctionType _option){
    bool success;
    debug("default doorF");
    switch (_option ){
    case CLOSE_:{
        success = door.rotateToPosition(value.LiftConfigs.door.close);
        break;
    }
    case OPEN_GET:{
        success = door.rotateToPosition(value.LiftConfigs.door.openGetObject);
        break;
    }
    case OPEN_LEAVE:{
        success = door.rotateToPosition(value.LiftConfigs.door.openLeaveObejct);
        break;
    }
    }
    //delayF(value.LiftConfigs.time.interval);
    //while(doorS()) delayF(10);
    return success;
}

bool AbstractLiftExecutor::handF(HandFunctioType _option){
    debug("default handF");
    bool success;
    switch( _option){
    case OPEN:{
        success = hand.rotateToPosition(value.LiftConfigs.hand.relese);
        break;
    }
    case CLOSE:{
        success = hand.rotateToPosition(value.LiftConfigs.hand.grab);
        break;
    }
    }
    //delayF(value.LiftConfigs.time.interval);
    //while(handS()) delayF(10);
    return success;
}

bool AbstractLiftExecutor::liftF(LiftFunctiontype _option){
    debug("default liftF");
    bool success;
    switch(_option){
    case LEVEL0:{
        success = lift.rotateToPosition(value.LiftConfigs.lift.level0);
        break;
    }
    case LEVEL1:{
        success =  lift.rotateToPosition(value.LiftConfigs.lift.level1);
        break;
    }
    case LEVEL2:{
        success = lift.rotateToPosition(value.LiftConfigs.lift.level2);
        break;
    }
    }
    //while(liftS()) delayF(10);
    delayF(value.LiftConfigs.time.adjust_position);
    return success;
}

void AbstractLiftExecutor::delayF(int _millisecondes){
    boost::this_thread::sleep(boost::posix_time::milliseconds(_millisecondes));
    return;
}

void AbstractLiftExecutor::delayF(){
    boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    return;
}

bool AbstractLiftExecutor::sensorF(){
    bool _objectOnsight;
    _objectOnsight = sensor.scanSensorStatus();
    return _objectOnsight;
}

bool AbstractLiftExecutor::liftS(){
    return lift.getServoStatus();
}

bool AbstractLiftExecutor::doorS(){
    return door.getServoStatus();
}

bool AbstractLiftExecutor::handS(){
    return hand.getServoStatus();
}


}
