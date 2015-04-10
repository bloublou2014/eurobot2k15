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

    pipeRight.rotateToPosition(value.FlapConfigs.flapRight.close);
    pipeLeft.rotateToPosition(value.FlapConfigs.flapLeft.close);

}

bool FlapExecutor::KickRightFunction(){
    debug("KickRight");
    bool success;

    success = pipeRight.rotateToPosition(value.FlapConfigs.flapRight.open);

    return success;
}

bool FlapExecutor::KickLeftFunction(){
    debug("KickLeft");
    bool success;

    success = pipeLeft.rotateToPosition(value.FlapConfigs.flapLeft.open);

    return success;
}

bool FlapExecutor::UnKickRightFunction(){
    debug("UnKickRight");
    bool success;

    success = pipeRight.rotateToPosition(value.FlapConfigs.flapRight.close);

    return success;
}

bool FlapExecutor::UnKickLeftFunction(){
    debug("UnKickLeft");
    bool success;

    success = pipeLeft.rotateToPosition(value.FlapConfigs.flapLeft.close);

    return success;
}

} // end namespace
