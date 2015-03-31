#include "LiftRightExecutor.h"

namespace executor {

string LiftRightExecutor::NAME = "LiftRightExecutor";

void LiftRightExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&LiftRightExecutor::processActuatorCommand));
    this->registerCommand(GetLiftState::NAME, static_cast<commandCallback>(&LiftRightExecutor::processGetLiftState));

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

    reload(&value, executorName);

    doorF(OPEN_GET);
    handF(OPEN);
    liftF(LEVEL0);

}

bool LiftRightExecutor::GetObjectFunction(){
    if (!lastState.Quantity ){
        lastState.Aveable = false;

        /*
        LiftNotification* liftNotification0 = new LiftNotification(lastState);
        sendNotification(liftNotification0);

        liftF(LEVEL0);     // pripremi se za hvatanje
        handF(OPEN);
        doorF(OPEN);
        delayF();

        while(!sensorF()) delayF(500);  // cekaj na senzor i onda pokreni hvatanje
        handF(CLOSE);
        liftF(LEVEL1);   // uhvati i digni
        delayF();

        doorF(CLOSE);    // zatvori door
        delayF();
 */
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

        lastState.Quantity = 1;
        lastState.Aveable = true;

        //LiftNotification* liftNotification1 = new LiftNotification(lastState);
        //sendNotification(liftNotification1);

        return true;

    }else if(lastState.Quantity < 4){
        stateLock.lock();
        lastState.Aveable = false;
        stateLock.unlock();

        //LiftNotification* liftNotification0 = new LiftNotification(lastState);
        //sendNotification(liftNotification0);
        /*
        liftF(LEVEL2);  // podesi visinu na mesto za stavljanje na drugi cunjic
        delayF();

        while(!sensorF()) delayF(500);
        doorF(OPEN);
        handF(OPEN);
        delayF();

        liftF(LEVEL0);
        delayF();

        handF(CLOSE);
        delayF();

        liftF(LEVEL2);
        delayF(2000);

        doorF(CLOSE);
        delayF();
*/

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


        LiftNotification* liftNotification1 = new LiftNotification(lastState);
        sendNotification(liftNotification1);

        return true;
    }else{
        error("NO MORE SPACE IN STORAGE");
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        return false;
    }
}

bool LiftRightExecutor::UnloadObjectFunction(){
    stateLock.lock();
    lastState.Aveable = false;
    stateLock.unlock();
    // TODO notification

    doorF(OPEN_LEAVE);
    handF(OPEN);
    liftF(LEVEL0);

    stateLock.lock();
    lastState.Quantity = 0;
    lastState.Aveable = true;
    stateLock.unlock();
    return true;
}


void LiftRightExecutor::processGetLiftState(Command* _command){
    debug("processing get lift  state");
    LiftNotification* liftNotification  = new LiftNotification(lastState);
    sendNotification(liftNotification);
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
