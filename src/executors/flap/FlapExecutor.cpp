#include "FlapExecutor.h"

namespace executor {

string FlapExecutor::NAME = "FlapExecutor";

void FlapExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&FlapExecutor::processActuatorCommand));
    // dodati registrovanje get state commande // TODO
}

void FlapExecutor::mapping(){

    pipeLeft.setServoSlaveAddress(char(1));
    pipeLeft.setServoPositionAddress(char(9));
    pipeLeft.setServoStatusSetAddress(char(14));
    pipeLeft.setServoStatusReadAddress(char(15));

    pipeRight.setServoSlaveAddress(char(1));
    pipeRight.setServoPositionAddress(char(8));
    pipeRight.setServoStatusSetAddress(char(16));
    pipeRight.setServoStatusReadAddress(char(17));

    reload(&value, executorName);

}

bool FlapExecutor::KickRightFunction(){
    debug("KickRight");

    pipeRight.rotateToPosition(value.FlapConfigs.flapRight.open);

    return true;
}

bool FlapExecutor::KickLeftFunction(){
    debug("KickLeft");

    pipeLeft.rotateToPosition(value.FlapConfigs.flapLeft.open);

    return true;
}

bool FlapExecutor::UnKickRightFunction(){
    debug("UnKickRight");

    pipeRight.rotateToPosition(value.FlapConfigs.flapRight.close);

    return true;
}

bool FlapExecutor::UnKickLeftFunction(){
    debug("UnKickLeft");
    pipeLeft.rotateToPosition(value.FlapConfigs.flapLeft.close);

    return true;
}

} // end namespace
