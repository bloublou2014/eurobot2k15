#include "ElectroSwitch.h"

namespace electro_switch{

void ElectroSwitch::setCoilSlaveAddress(unsigned char _slave_address){
    this->slaveAddress = _slave_address;
}

void ElectroSwitch::setCoilAddress(unsigned short _address){
    this->address = _address;
}

bool ElectroSwitch::setCoilState(bool _status){
    bool success;
    if(_status){
        success = modbus->setCoil(slaveAddress, address, 1, false );
    }else{
        success = modbus->setCoil(slaveAddress, address, 0, false );
    }
    return success;
}

}
