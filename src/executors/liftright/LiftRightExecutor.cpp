#include "LiftRightExecutor.h"

namespace executor {

string LiftRightExecutor::NAME = "LiftRightExecutor";

void LiftRightExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&LiftRightExecutor::processActuatorCommand));
    //modbusClient = ModbusSensorClient::getModbusSensorInstance();
    //readingSensore = true;
    lastState.Aveable = true;
    lastState.Quantity = 0;
    executorName = this->NAME;
}

void LiftRightExecutor::mapping(){

    hand.setServoSlaveAddress(char(1));
    hand.setServoPositionAddress(char(2));
    hand.setServoStatusSetAddress(char(4));
    hand.setServoStatusReadAddress(char(5));

    lift.setServoSlaveAddress(char(1));
    lift.setServoPositionAddress(char(4));
    lift.setServoStatusSetAddress(char(8));
    lift.setServoStatusReadAddress(char(9));

    door.setServoSlaveAddress(char(1));
    door.setServoPositionAddress(char(7));
    door.setServoStatusSetAddress(char(14));
    door.setServoStatusReadAddress(char(15));

//    sensor.setSlaveAddress(char(4));
//    sensor.setScanAddress(char(3));
    //modbusClient->registerToSensoreCallback(char(4),char(3),true, this);
    liftSensor.setConfig(char(4),char(3),1,this, false);
    liftSensor.RegisterSensor();

    reload(&value, executorName);

    doorF(OPEN_GET);
    handF(CLOSE);
    //liftF(value.LiftConfigs.lift.levelBall);
    liftF(LEVEL2);

}

bool LiftRightExecutor::GetObjectFunction(){
    debug("LiftRight Get");
    shoulGetObject = true;
    //readingSensore = true;
    //readingSensore = true;
    liftSensor.StartSensor();

    return true;
}

bool LiftRightExecutor::liftProcess(){
    debug("DOING SOMETHING");

    if(lastState.Quantity < 3 && shoulGetObject){
        debug("FIRST ONE ");
        stateLock.lock();
       //readingSensore = false;
        stateLock.unlock();

        // NEW CONFIG

        doorF(OPEN_GET);
        handF(OPEN);
        liftF(LEVEL0);
        handF(CLOSE);
        delayF(value.LiftConfigs.time.handOpenClose);
        LiftProgressNotification* notification = new LiftProgressNotification(LiftProgressNotification::ProgressType::ROLLER_CATCHED);
        sendNotification(notification);
        liftF(LEVEL2);
        doorF(CLOSE_);


        int count=0;
        stateLock.lock();
        lastState.Quantity++;
        lastState.Aveable = true;
        //readingSensore = true;
        count=lastState.Quantity;
        stateLock.unlock();

        //Send Notification
        LIftNotification* liftNotification=new LIftNotification(LIftNotification::Side::RIGHT, count);
        sendNotification(liftNotification);

        return true;

    }else if(lastState.Quantity == 3 && shoulGetObject){

        debug(" SECOND .... ");

        stateLock.lock();
        lastState.Aveable = false;
        stateLock.unlock();

        doorF(OPEN_GET);
        handF(OPEN);
        liftF(LEVEL0);
        handF(CLOSE);
        delayF(value.LiftConfigs.time.handOpenClose);
        LiftProgressNotification* notification = new LiftProgressNotification(LiftProgressNotification::ProgressType::ROLLER_CATCHED);
        sendNotification(notification);
//        liftF(LEVEL1);
        doorF(CLOSE_);

        delayF(value.LiftConfigs.time.doorOpenClose);


        stateLock.lock();
        lastState.Quantity++;
        lastState.Aveable = true;
        //readingSensore = true;
        stateLock.unlock();

        //Send notification
        LIftNotification* liftNotification=new LIftNotification(LIftNotification::Side::RIGHT, 4);
        sendNotification(liftNotification);

        return true;



    }else if(lastState.Quantity < 5 && shoulGetObject){
        //readingSensore = false;
        error("NO MORE SPACE IN STORAGE");
        return true;
    }

    //debug("LR CANT DOIT");
    return false;

}

bool LiftRightExecutor::UnloadObjectFunction(){
    stateLock.lock();
    lastState.Aveable = false;
    stateLock.unlock();
    // TODO notification

    doorF(OPEN_LEAVE);
    liftF(LEVEL0);
    handF(OPEN);


    stateLock.lock();
    lastState.Quantity = 0;
    lastState.Aveable = true;
    //readingSensore = true;
    stateLock.unlock();

    //Send notification
    LIftNotification* liftNotification=new LIftNotification(LIftNotification::Side::RIGHT, 0);
    sendNotification(liftNotification);

    return true;
}


void LiftRightExecutor::processGetLiftState(Command* _command){
    debug("processing get lift  state");
    GetLiftStateResponse* resp = new GetLiftStateResponse(_command->getSource(), _command->getDestination(),lastState);
    resp->setId(_command->getId());
    sendResponse(resp);
}


bool LiftRightExecutor::CallbackGetRightFunction(){
    debug("executeing lift right callback command");
    bool success = false;

    success = this->liftProcess();
    /*
    if(success) readingSensore = true;
    else readingSensore = false;
    */
    if(success){
        liftSensor.StartSensor();
    }else{
        liftSensor.StopSensor();
    }
}

/*
 void LiftRightExecutor::ProcessLiftRightSensoreCallback(){
     std::cout << "LIRT RIGHT" << std::endl;
     //readingSensore = true;
     Command* cmd = ActuatorAction::LiftRight(CALLBACK_GET_RIGHT);
         commandQueueLock.lock();
         commandsToProcess.push(Instruction(cmd));
         commandQueueLock.unlock();
         queueNotEmpty.notify_one();
 }
*/

void LiftRightExecutor::SensorDriverCallback(int _id, bool _detected){
    std::cout << "LIRT RIGHT" << std::endl;
    LiftProgressNotification* liftNotification = new LiftProgressNotification(LiftProgressNotification::ProgressType::SENSOR_TRIGGERED);
    sendNotification(liftNotification);
    Command* cmd = ActuatorAction::LiftRight(CALLBACK_GET_RIGHT);
        commandQueueLock.lock();
        commandsToProcess.push(Instruction(cmd));
        commandQueueLock.unlock();
        queueNotEmpty.notify_one();
}






} // namespace end
