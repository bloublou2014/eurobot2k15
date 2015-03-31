#include "LiftCenterExecutor.h"


namespace executor {

string LiftCenterExecutor::NAME = "LiftCenterExecutor";

void LiftCenterExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&LiftCenterExecutor::processActuatorCommand));
    // dodati registrovanje get state commande

}

}
