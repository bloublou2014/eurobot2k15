#include "PopcornExecutor.h"

namespace executor {

string PopcornExecutor::NAME = "PopcornExecutor";

void PopcornExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&PopcornExecutor::processActuatorCommand));
    // dodati registrovanje get state commande
}


void PopcornExecutor::mapping(){
    pipeLeft.setServoSlaveAddress(char(1));
    pipeLeft.setServoPositionAddress(char(9));
    pipeLeft.setServoStatusSetAddress(char(18));
    pipeLeft.setServoStatusReadAddress(char(19));

    pipeRight.setServoSlaveAddress(char(1));
    pipeRight.setServoPositionAddress(char(8));
    pipeRight.setServoStatusSetAddress(char(16));
    pipeRight.setServoStatusSetAddress(char(17));

    backDoor.setServoSlaveAddress(char(1));
    backDoor.setServoPositionAddress(char(3));
    backDoor.setServoStatusSetAddress(char(6));
    backDoor.setServoStatusReadAddress(char(7));

     reload(&value, executorName);
}


bool PopcornExecutor::GetPopcornFunction(){
    debug("GetPopcoron");
    pipeLeft.rotateToPosition(value.PopcornConfigs.pipeLeft.open);
    pipeRight.rotateToPosition(value.PopcornConfigs.pipeRight.open);

    boost::this_thread::sleep(boost::posix_time::milliseconds(value.PopcornConfigs.time.interval));

    pipeLeft.rotateToPosition(value.PopcornConfigs.pipeLeft.close);
    pipeRight.rotateToPosition(value.PopcornConfigs.pipeRight.close);

    return true;

}

bool PopcornExecutor::UnloadPopcornFunction(){
    debug("UnloadPopcorn");

    backDoor.rotateToPosition(value.PopcornConfigs.backDoor.open);

    boost::this_thread::sleep(boost::posix_time::milliseconds(value.PopcornConfigs.time.interval));

    backDoor.rotateToPosition(value.PopcornConfigs.backDoor.close);

    return false;
}




} // endNamespace
