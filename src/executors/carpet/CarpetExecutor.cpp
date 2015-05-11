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
    el_switchLeft.setCoilAddress(char(7));

    //el_switchRight.setCoilSlaveAddress(char(1));
    //el_switchRight.setCoilAddress(char(9));

     reload(&value,this->NAME );

     //armLeft.rotateToPosition(value.CarpetConfig.armLeft.close);
     //armRight.rotateToPosition(value.CarpetConfig.armRight.close);

     //el_switchLeft.setCoilState(false);
     //el_switchRight.setCoilState(false);

}

bool CarpetExecutor::LeaveCarpetFunction(){

    armLeft.rotateToPosition(value.CarpetConfig.armLeft.positionOpen);
    armRight.rotateToPosition(value.CarpetConfig.armRight.positionOpen);

    boost::this_thread::sleep(boost::posix_time::milliseconds(value.CarpetConfig.armOpenCloseTime));

    el_switchLeft.setCoilState(true);
    //el_switchRight.setCoilState(true);

    boost::this_thread::sleep(boost::posix_time::milliseconds(value.CarpetConfig.leaveCarpetTime));

    armLeft.rotateToPosition(value.CarpetConfig.armLeft.positionClose);
    armRight.rotateToPosition(value.CarpetConfig.armRight.positionClose);

    return true;

}

bool CarpetExecutor::CarpetLeaveFunction(){
     return el_switchLeft.setCoilState(true);
     
}

bool CarpetExecutor::CarpetPosition1Function(){
    bool successLeft = armLeft.rotateToPosition(value.CarpetConfig.armLeft.position1);
    bool successRight = armRight.rotateToPosition(value.CarpetConfig.armRight.position1);
    if(successLeft && successRight) return true;
    else return false;
}

bool CarpetExecutor::CarpetPosition2Function(){
    bool successLeft = armLeft.rotateToPosition(value.CarpetConfig.armLeft.position2);
    bool successRight = armRight.rotateToPosition(value.CarpetConfig.armRight.position2);
    if(successLeft && successRight) return true;
    else return false;
}

bool CarpetExecutor::CarpetPositionOpenFunction(){
    bool successLeft = armLeft.rotateToPosition(value.CarpetConfig.armLeft.positionOpen);
    bool successRight = armRight.rotateToPosition(value.CarpetConfig.armRight.positionOpen);
    if(successLeft && successRight) return true;
    else return false;
}

bool CarpetExecutor::CarpetPositionCloseFunction(){
    bool successLeft = armLeft.rotateToPosition(value.CarpetConfig.armLeft.positionClose);
    bool successRight = armRight.rotateToPosition(value.CarpetConfig.armRight.positionClose);
    if(successLeft && successRight) return true;
    else return false;
}
}
