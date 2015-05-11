#include "LiftLeftExecutor.h"

namespace executor {

string LiftLeftExecutor::NAME = "LiftLeftExecutor";

void LiftLeftExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&LiftLeftExecutor::processActuatorCommand));
    //modbusClient = ModbusSensorClient::getModbusSensorInstance();
    //this->readingSensore = true;
    lastState.Aveable = true;
    lastState.Quantity = 0;
    executorName = this->NAME;

}

void LiftLeftExecutor::mapping(){

    hand.setServoSlaveAddress(char(1));
    hand.setServoPositionAddress(char(5));
    hand.setServoStatusSetAddress(char(10));
    hand.setServoStatusReadAddress(char(11));

    lift.setServoSlaveAddress(char(1));
    lift.setServoPositionAddress(char(1));
    lift.setServoStatusSetAddress(char(2));
    lift.setServoStatusReadAddress(char(3));

    door.setServoSlaveAddress(char(1));
    door.setServoPositionAddress(char(6));
    door.setServoStatusSetAddress(char(12));
    door.setServoStatusReadAddress(char(13));

    //sensor.setSlaveAddress(char(4));
    //sensor.setScanAddress(char(4));
    //modbusClient->registerToSensoreCallback(char(4),char(4), true, this);
    //modbusClient->registerCoilReading(this,char(4),char(4));
    liftSensor.setConfig(char(4),char(4),1,this,false);
    liftSensor.RegisterSensor();

    reload(&value, executorName);

    doorF(OPEN_GET);
    handF(CLOSE);
    //lift.rotateToPosition(value.LiftConfigs.lift.levelBall);
    liftF(LEVEL2);
}


bool LiftLeftExecutor::GetObjectFunction(){
    debug("LiftLeft Get");
    shoulGetObject = true;
    //readingSensore = true;
    liftSensor.StartSensor();


    return true;

}

bool LiftLeftExecutor::liftProcess(){
    //debug("DOING SOMETHING");

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
            liftF(LEVEL2);
            doorF(CLOSE_);


            int count=0;
            stateLock.lock();
            lastState.Quantity++;
            lastState.Aveable = true;
            //readingSensore = true;
            //readingSensore = true;
            count=lastState.Quantity;
            stateLock.unlock();

            LIftNotification* liftNotification=new LIftNotification(LIftNotification::Side::LEFT, count);
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
            liftF(LEVEL1);
            doorF(CLOSE_);

            delayF(value.LiftConfigs.time.doorOpenClose);


            stateLock.lock();
            lastState.Quantity++;
            lastState.Aveable = true;
            //readingSensore = true;
            stateLock.unlock();

            LIftNotification* liftNotification=new LIftNotification(LIftNotification::Side::LEFT, 4);
            sendNotification(liftNotification);

            return true;


        }else if(lastState.Quantity < 5  && shoulGetObject){
            //readingSensore = false;
            error("NO MORE SPACE IN STORAGE");
            //sendResponseFromCommand(currentActuatorCommand, ERROR);
            return false;
        }

        return false;

    //debug("LL CANT DOIT");
}



bool LiftLeftExecutor::UnloadObjectFunction(){
    stateLock.lock();
    lastState.Aveable = false;
    stateLock.unlock();
    // TODO notification

    //doorF(OPEN);
    debug("UNLOADING");
    doorF(OPEN_LEAVE);
    liftF(LEVEL0);
    handF(OPEN);

    stateLock.lock();
    lastState.Quantity = 0;
    lastState.Aveable = true;
    //readingSensore = true;

    stateLock.unlock();

    LIftNotification* liftNotification=new LIftNotification(LIftNotification::Side::LEFT, 0);
    sendNotification(liftNotification);

    return true;
}


void LiftLeftExecutor::processGetLiftState(Command* _command){
    debug("processGetLiftState");

    GetLiftStateResponse* resp = new GetLiftStateResponse(_command->getSource(), _command->getDestination(),lastState);
    resp->setId(_command->getId());
    sendResponse(resp);

}


bool LiftLeftExecutor::CallbackGetLeftFunction(){
    debug("executeing lift left callback command");
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
    return success;
}

/*
void LiftLeftExecutor::ProcessLiftLeftSensoreCallback(){

    std::cout << "LIRT LEFT" << std::endl;
    //readingSensore = true;
    Command* cmd = ActuatorAction::LiftLeft(CALLBACK_GET_LEFT);
        commandQueueLock.lock();
        commandsToProcess.push(Instruction(cmd));
        commandQueueLock.unlock();
        queueNotEmpty.notify_one();
}
*/

void LiftLeftExecutor::SensorDriverCallback(int _id, bool _detected){
    std::cout << "LIRT LEFT" << std::endl;
    //readingSensore = true;
    Command* cmd = ActuatorAction::LiftLeft(CALLBACK_GET_LEFT);
        commandQueueLock.lock();
        commandsToProcess.push(Instruction(cmd));
        commandQueueLock.unlock();
        queueNotEmpty.notify_one();
}



} // end namespace
