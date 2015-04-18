#ifndef _ELECTRO_SWITCH
#define _ELECTRO_SWITCH

#include "utils/modbus/ModbusClient.h"

using namespace modbus;

namespace electro_switch {


class ElectroSwitch{
private:
    ModbusClient* modbus;
    unsigned char slaveAddress;
    unsigned short address;
public:
    ElectroSwitch(){}

    void setCoilSlaveAddress(unsigned char _slave_address);
    void setCoilAddress(unsigned short _address);
    void setCoilState(bool _state);

};


} // end namespace




#endif // _ELECTRO_SWITCH
