#include "AbstractLiftExecutor.h"

namespace executor {

void AbstractLiftExecutor::maping(){
    debug("default maping");


}

void AbstractLiftExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&AbstractLiftExecutor::processActuatorCommand));
    //modbusClient->registerToSensoreCallback(char(4),char(4),true,this);
    //modbusClient->registerToSensoreCallback(char(4),char(3),true,this);
}


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

bool AbstractLiftExecutor::GetObjectStopFunction(){
    shoulGetObject = false;
    return true;
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
    //debug("default doorF");
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
    //debug("default handF");
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
    //debug("default liftF");
    bool success;
    switch(_option){
    case LEVEL0:{
        success = lift.rotateToPosition(value.LiftConfigs.lift.level0);
        if (currentLevel > LEVEL0) delayF(value.LiftConfigs.time.liftDown);
        else if(currentLevel < LEVEL0) delayF(value.LiftConfigs.time.liftUp);
        currentLevel = LEVEL0;
        break;
    }
    case LEVEL1:{
        success =  lift.rotateToPosition(value.LiftConfigs.lift.level1);
        if (currentLevel > LEVEL1) delayF(value.LiftConfigs.time.liftDown);
        else if(currentLevel < LEVEL1) delayF(value.LiftConfigs.time.liftUp);
        currentLevel = LEVEL1;
        break;
    }
    case LEVEL2:{
        success = lift.rotateToPosition(value.LiftConfigs.lift.level2);
        if (currentLevel > LEVEL2) delayF(value.LiftConfigs.time.liftDown);
        else if(currentLevel < LEVEL2) delayF(value.LiftConfigs.time.liftUp);
        currentLevel = LEVEL2;
        break;
    }
    }
    //while(liftS()) delayF(10);
    //delayF(value.LiftConfigs.time.adjust_position);
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
/*
void AbstractLiftExecutor::ProcessLiftRightSensoreCallback(){
    Command* cmd = ActuatorAction::LiftRight(CALLBACK_GET);
        commandQueueLock.lock();
        commandsToProcess.push(Instruction(cmd));
        commandQueueLock.unlock();
        queueNotEmpty.notify_one();
    }

void AbstractLiftExecutor::ProcessLiftLeftSensoreCallback(){
    Command* cmd = ActuatorAction::LiftLeft(CALLBACK_GET);
        commandQueueLock.lock();
        commandsToProcess.push(Instruction(cmd));
        commandQueueLock.unlock();
        queueNotEmpty.notify_one();
    }

bool AbstractLiftExecutor::CallbackGetFunction(){
    bool success = false;
    success = this->liftProcess();
    if(success) readingSensore = true;
    else readingSensore = false;

}
*/

bool AbstractLiftExecutor::liftProcess(){

}

}
