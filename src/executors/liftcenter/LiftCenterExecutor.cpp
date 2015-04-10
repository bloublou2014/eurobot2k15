#include "LiftCenterExecutor.h"


namespace executor {

string LiftCenterExecutor::NAME = "LiftCenterExecutor";

void LiftCenterExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&LiftCenterExecutor::processActuatorCommand));
    // dodati registrovanje get state commande
}

void LiftCenterExecutor::mapping(){

    hand.setServoSlaveAddress(char(1));
    hand.setServoPositionAddress(char(50));

    reload(&value, LiftCenterExecutor::NAME);

   //hand.rotateToPosition(value.LiftConfigs.hand.relese);

}

bool LiftCenterExecutor::GetObjectFunction(){
    debug("Get Object");
    bool success;

    success = hand.rotateToPosition(value.LiftConfigs.hand.grab);

    return success;
}

bool LiftCenterExecutor::UnloadObjectFunction(){
    debug("unload Function");
    bool success;

    success = hand.rotateToPosition(value.LiftConfigs.hand.relese);

    return success;
}



} // end namespace
