#include "LiftLeftExecutor.h"

namespace executor {

string LiftLeftExecutor::NAME = "LiftLeftExecutor";

void LiftLeftExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&LiftLeftExecutor::processActuatorCommand));
    this->registerCommand(GetLiftState::NAME, static_cast<commandCallback>(&LiftLeftExecutor::processGetLiftState));

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

    reload(&value, executorName);

    doorF(OPEN_GET);
    handF(OPEN);
    liftF(LEVEL0);
}


bool LiftLeftExecutor::GetObjectFunction(){
    if (!lastState.Quantity ){
        lastState.Aveable = false;


       // LiftNotification* liftNotification0 = new LiftNotification(lastState);
        //sendNotification(liftNotification0);


        liftF(LEVEL0);
        handF(CLOSE);

        delayF(value.LiftConfigs.time.interval);
        doorF(OPEN_GET);
        liftF(LEVEL1);
        doorF(CLOSE_);
        delayF(value.LiftConfigs.time.interval);

        handF(OPEN);
        delayF(value.LiftConfigs.time.some_time);
        liftF(LEVEL0);




        lastState.Aveable = true;
        lastState.Quantity++;

       // LiftNotification* liftNotification1 = new LiftNotification(lastState);
        //sendNotification(liftNotification1);

        return true;

    }else if(lastState.Quantity < 4){
        stateLock.lock();
        lastState.Aveable = false;
        stateLock.unlock();
        //LiftNotification* liftNotification0 = new LiftNotification(lastState);
        //sendNotification(liftNotification0);


        liftF(LEVEL0);
        handF(CLOSE);

        delayF(value.LiftConfigs.time.interval);
        doorF(OPEN_GET);
        liftF(LEVEL1);
        doorF(CLOSE_);
        delayF(value.LiftConfigs.time.interval);

        handF(OPEN);
        delayF(value.LiftConfigs.time.some_time);
        liftF(LEVEL0);

        stateLock.lock();
        lastState.Quantity++;
        lastState.Aveable = true;
        stateLock.unlock();
        //LiftNotification* liftNotification1 = new LiftNotification(lastState);
        //sendNotification(liftNotification1);

        return true;
    }else{
        error("NO MORE SPACE IN STORAGE");
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        return false;
    }
}

bool LiftLeftExecutor::UnloadObjectFunction(){
        lastState.Aveable = false;

        // TODO notification

        //doorF(OPEN);
        doorF(OPEN_LEAVE);
        handF(OPEN);
        liftF(LEVEL0);

        lastState.Quantity = 0;
        lastState.Aveable = true;

        return true;
}


void LiftLeftExecutor::processGetLiftState(Command* _command){
    debug("processGetLiftState");
   // LiftNotification* notify = new LiftNotification(lastState);
    //sendNotification(notify);


    GetLiftStateResponse* resp = new GetLiftStateResponse(_command->getSource(), _command->getDestination(),lastState);
    resp->setId(_command->getId());
    sendResponse(resp);

}

} // end namespace
