#include "LiftLeftExecutor.h"

namespace executor {

string LiftLeftExecutor::NAME = "LiftLeftExecutor";

void LiftLeftExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&LiftLeftExecutor::processActuatorCommand));
    //this->registerCommand(GetLiftState::NAME, static_cast<commandCallback>(&LiftLeftExecutor::processGetLiftState));
    //TODO zakimentarisano jer nije uradjeno preslikavanje u JS
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

    sensor.setSlaveAddress(char(4));
    sensor.setScanAddress(char(4));


    reload(&value, executorName);

    doorF(OPEN_GET);
    handF(OPEN);
    liftF(LEVEL0);
}


bool LiftLeftExecutor::GetObjectFunction(){
    debug("LiftLeft Get");
    while (lastState.Quantity < 5 && !shouldStop ) {
        /*
        while(sensorDetect == false && !shouldStop){
            delayF(50);
            sensorDetect = sensor.scanSensorStatus();
            debug("respin");
        }
        */
         while(!sensor.scanSensorStatus() && !shouldStop) delayF(100);

        if (!lastState.Quantity && !shouldStop){
            stateLock.lock();
            lastState.Aveable = false;
            stateLock.unlock();

            // NEW CONFIG

            handF(CLOSE);
            liftF(LEVEL2);
            doorF(CLOSE_);
            delayF(value.LiftConfigs.time.doorOpenClose);

            stateLock.lock();
            lastState.Quantity++;
            lastState.Aveable = true;
            stateLock.unlock();


            //return true;

        }else if(lastState.Quantity < 4 && !shouldStop){
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

            delayF(value.LiftConfigs.time.doorOpenClose);


            stateLock.lock();
            lastState.Quantity++;
            lastState.Aveable = true;
            stateLock.unlock();


            //return true;
        }else{
            error("NO MORE SPACE IN STORAGE");
            sendResponseFromCommand(currentActuatorCommand, ERROR);
            return true;
        }

    }
}

bool LiftLeftExecutor::UnloadObjectFunction(){
    stateLock.lock();
    lastState.Aveable = false;
    stateLock.unlock();
    // TODO notification

    //doorF(OPEN);
    doorF(OPEN_LEAVE);
    liftF(LEVEL0);
    handF(OPEN);

    stateLock.lock();
    lastState.Quantity = 0;
    lastState.Aveable = true;
    stateLock.unlock();

    return true;
}


void LiftLeftExecutor::processGetLiftState(Command* _command){
    debug("processGetLiftState");

    GetLiftStateResponse* resp = new GetLiftStateResponse(_command->getSource(), _command->getDestination(),lastState);
    resp->setId(_command->getId());
    sendResponse(resp);

}

} // end namespace
