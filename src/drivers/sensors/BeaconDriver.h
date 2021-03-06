#ifndef _BEACON_DRIVER
#define _BEACON_DRIVER

#include "utils/modbus/ModbusClientSwitzerland.h"

using namespace modbus;

namespace sensor{

class BeaconDriverInterface{
public:
    virtual void beaconDriverCallbackVeliki(short _veliki_cordX, short _veliki_cordY, bool _running){}
    virtual void beaconDriverCallbackMali(short _mali_cordX, short _mali_cordY, bool _running){}
};

class BeaconDriver: public ModbusClientSWInterface, public BeaconDriverInterface{
private:
    struct ModID{
        unsigned char slave_address;
        short function_address_cordX;
        short function_address_cordY;
        int modID_cordX;
        int modID_cordY;
        short cordX;
        short cordY;
        bool settedX = false;
        bool settedY = false;
        bool running = false;
    };
    struct ConfigID{
        unsigned char slave_address;
        short power_address;
        short numberOfBeacons_address;
        int numberOfBeacons;
    };

public:
    BeaconDriver();
    setBeaconConfig(unsigned char _slave_address_veliki, short _function_address_cordX_veliki, short _function_address_cordY_veliki,
                    unsigned char _slave_address_mali, short _function_address_cordX_mali, short _function_address_cordY_mali,
                    unsigned char _slave_address_funtions, short _power_address, short _number_address, int _numberOfBeacons,
                    BeaconDriverInterface* _interface );
    void setBeaconNumber(int _number);

    void registerBeacon();
    void startBeacon();
    void stopBeacon();


private:
    ModbusClientSW* modbusClient;
    BeaconDriverInterface* interface;
    ModID beaconMali;
    ModID beaconVeliki;
    ConfigID beaconConfig;


    void callbackRegisterFunction(int _mapID, short _data);
};
} // end namespace
#endif // _BEACON_DRIVER
