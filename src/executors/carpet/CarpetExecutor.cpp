#include "CarpetExecutor.h"

namespace executor {

string CarpetExecutor::NAME = "CarpetExecutor";

void CarpetExecutor::suscribe(){
    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&CarpetExecutor::processActuatorCommand));


}

void CarpetExecutor::mapping(){
    armLeft.setServoSlaveAddress(char(1));
    armLeft.setServoPositionAddress(char(2));

    armRight.setServoSlaveAddress(char(1));
    armRight.setServoPositionAddress(char(1));

    el_switchLeft.setCoilSlaveAddress(char(1));
    el_switchLeft.setCoilAddress(char(8));

    el_switchRight.setCoilSlaveAddress(char(1));
    el_switchRight.setCoilAddress(char(9));

     reload(&value,this->NAME );

     armLeft.rotateToPosition(value.CarpetConfig.armLeft.close);
     armRight.rotateToPosition(value.CarpetConfig.armRight.close);

     el_switchLeft.setCoilState(false);
     el_switchRight.setCoilState(false);

}

bool CarpetExecutor::LeaveCarpetFunction(){

    armLeft.rotateToPosition(value.CarpetConfig.armLeft.open);
    armRight.rotateToPosition(value.CarpetConfig.armRight.open);

    boost::this_thread::sleep(boost::posix_time::milliseconds(value.CarpetConfig.armOpenCloseTime));

    el_switchLeft.setCoilState(true);
    el_switchRight.setCoilState(true);

    boost::this_thread::sleep(boost::posix_time::milliseconds(value.CarpetConfig.leaveCarpetTime));

    armLeft.rotateToPosition(value.CarpetConfig.armLeft.close);
    armRight.rotateToPosition(value.CarpetConfig.armRight.close);

    return true;

}


}
