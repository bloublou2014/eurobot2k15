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
    modbusClient = ModbusSensorClient::getModbusSensorInstance();
    modbusClient->registerToSensoreCallback(char(4),char(3),true, this);
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
    sensor.setScanAddress(char(3));

    reload(&value, executorName);

    doorF(OPEN_GET);
    handF(OPEN);
    liftF(LEVEL0);

}

bool LiftRightExecutor::GetObjectFunction(){
    debug("LiftRight Get");
    shoulGetObject = true;
    readingSensore = true;
    return true;
}

bool LiftRightExecutor::liftProcess(){
    debug("DOING SOMETHING");

        if(lastState.Quantity < 3){
            debug("FIRST ONE ");
            stateLock.lock();
            readingSensore = false;
            stateLock.unlock();

            // NEW CONFIG

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
            readingSensore = true;
            stateLock.unlock();

        }else if(lastState.Quantity == 3){

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
            readingSensore = false;
            //readingSensore = true;
            stateLock.unlock();


        }else if(lastState.Quantity < 5 ){
            readingSensore = false;
            error("NO MORE SPACE IN STORAGE");
            //sendResponseFromCommand(currentActuatorCommand, ERROR);
            return true;
        }

    debug("LR CANT DOIT");

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
    GetLiftStateResponse* resp = new GetLiftStateResponse(_command->getSource(), _command->getDestination(),lastState);
    resp->setId(_command->getId());
    sendResponse(resp);

}

void LiftRightExecutor::brodcastNotification(){

}


bool LiftRightExecutor::liftLoop(){
    if(shoulGetObject && !shouldStop && sensoreCallbackRecived){
        sensoreCallbackRecived = false;
        liftProcess();
    }
}


void LiftRightExecutor::ProcessSensorCallback(){
    if ( lastState.Aveable ) {
        sensoreCallbackRecived = true;
        debug("LR DETECTEC OBJECT");
    }
    return;
}

} // namespace end
