#ifndef _BRKON_DRIVER
#define _BRKON_DRIVER

#include "utils/modbus/ModbusClientSwitzerland.h"

using namespace modbus;
namespace sensor {

class BrkonDriverInterface{
public:
    virtual void brkonDriverCallback(short _dataFront, short _dataBack, bool _detected){ return;}
};

class BrkonDriver : public ModbusClientSWInterface, public BrkonDriverInterface{
private:
    struct ModID{
        unsigned char slave_address;
        short function_address;
        int modID;
    };

public:
    BrkonDriver(){
        modbusClient =  ModbusClientSW::getModbusClientInstance();
    }
    //BrkonDriver();
    void startBrkon();
    void stopBrkon();
    void registerBrkon();
    void setCoilConfig(unsigned char _slave_address, short _function_address);
    void setRegisterConfig(unsigned char _slave_address, short _function_address);
    void setPowerCoilConfig(unsigned char _slave_address, short _function_address);
    void registerInerface(BrkonDriverInterface *_callback_interface);

private:
    ModbusClientSW* modbusClient;
    BrkonDriverInterface* callbakInterface;
    ModID coilID;
    ModID registerID;
    ModID powerID;
    short data;
    bool detected;

    void callbackCoilFunction(int _mapID, bool _detected);
    void callbackRegisterFunction(int _mapID, short _data);
    void readRegister();


};

}

#endif // end _BRKON_DRIVER
