#include "ElectroSwitch.h"

namespace electro_switch{

void ElectroSwitch::setCoilSlaveAddress(unsigned char _slave_address){
    this->slaveAddress = _slave_address;
}

void ElectroSwitch::setCoilAddress(unsigned short _address){
    this->address = _address;
}

void ElectroSwitch::setCoilState(bool _status){
    if(_status){
        modbus->setCoil(slaveAddress, address, 1 );
    }else{
        modbus->setCoil(slaveAddress, address, 0 );

    }
}

}
