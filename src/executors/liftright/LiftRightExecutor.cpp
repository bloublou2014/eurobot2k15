#include "LiftRightExecutor.h"

namespace executor {

string LiftRightExecutor::NAME = "LiftRightExecutor";

void LiftRightExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&LiftRightExecutor::processActuatorCommand));
    //this->registerCommand(GetLiftState::NAME, static_cast<commandCallback>(&LiftRightExecutor::processGetLiftState));
    // TODO zakomentarisano jer nije uradjeno preslikavanje u js
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

    sensor.setSlaveAddress(char(4));
    sensor.setScanAddress(char(1));

    reload(&value, executorName);

    doorF(OPEN_GET);
    handF(OPEN);
    liftF(LEVEL0);

}

bool LiftRightExecutor::GetObjectFunction(){
    while(lastState.Quantity < 4){
        while(!sensor.scanSensorStatus()) delayF(40);

        if (!lastState.Quantity ){
            stateLock.lock();
            lastState.Aveable = false;
            stateLock.unlock();
            /*
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
    */

            handF(CLOSE);
            liftF(LEVEL2);
            doorF(CLOSE_);

            stateLock.lock();
            lastState.Quantity++;
            lastState.Aveable = true;
            stateLock.unlock();
            return true;

        }else if(lastState.Quantity < 4){
            stateLock.lock();
            lastState.Aveable = false;
            stateLock.unlock();


            /*
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
    */
            doorF(OPEN_GET);
            handF(OPEN);
            liftF(LEVEL0);
            handF(CLOSE);
            delayF(value.LiftConfigs.time.handOpenClose);
            liftF(LEVEL2);
            doorF(CLOSE_);


            stateLock.lock();
            lastState.Quantity++;
            lastState.Aveable = true;
            stateLock.unlock();



            return true;
        }else{
            error("NO MORE SPACE IN STORAGE");
            sendResponseFromCommand(currentActuatorCommand, ERROR);
            return false;
        }
    }
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
    stateLock.unlock();
    return true;
}


void LiftRightExecutor::processGetLiftState(Command* _command){
    debug("processing get lift  state");
    //LiftNotification* liftNotification  = new LiftNotification(lastState);
    //sendNotification(liftNotification);
    GetLiftStateResponse* resp = new GetLiftStateResponse(_command->getSource(), _command->getDestination(),lastState);
    resp->setId(_command->getId());
    sendResponse(resp);

}

void LiftRightExecutor::brodcastNotification(){
    //debug("notification send");
    //LiftNotification* liftNotification = new LiftNotification(lastState);
    //sendNotification(liftNotification);
    //Notification* m = new Notification("liftNotification","asd");
    //sendNotification(m);

}

} // namespace end
