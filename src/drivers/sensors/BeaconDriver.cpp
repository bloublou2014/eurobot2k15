#include "BeaconDriver.h"

namespace sensor{

BeaconDriver::BeaconDriver(){
    modbusClient = ModbusClientSW::getModbusClientInstance();
}

BeaconDriver::setBeaconConfig(unsigned char _slave_address_veliki, short _function_address_cordX_veliki, short _function_address_cordY_veliki,
                              unsigned char _slave_address_mali, short _function_address_cordX_mali, short _function_address_cordY_mali,
                              unsigned char _slave_address_funtions, short _power_address, short _number_address,
                              int _numberOfBeacons,
                              BeaconDriverInterface* _interface ){
    beaconVeliki.slave_address = _slave_address_veliki;
    beaconVeliki.function_address_cordX = _function_address_cordX_veliki;
    beaconVeliki.function_address_cordY = _function_address_cordY_veliki;

    beaconMali.slave_address = _slave_address_mali;
    beaconMali.function_address_cordX = _function_address_cordX_mali;
    beaconMali.function_address_cordY = _function_address_cordY_mali;

    beaconConfig.slave_address = _slave_address_funtions;
    beaconConfig.power_address = _power_address;
    beaconConfig.numberOfBeacons_address = _number_address;
    beaconConfig.numberOfBeacons = _numberOfBeacons;

    interface = _interface;
}

void BeaconDriver::registerBeacon(){
    beaconVeliki.modID_cordX = modbusClient->registerRegisterReading(this,beaconVeliki.slave_address,beaconVeliki.function_address_cordX);
    beaconVeliki.modID_cordY = modbusClient->registerRegisterReading(this,beaconVeliki.slave_address,beaconVeliki.function_address_cordY);

    beaconMali.modID_cordX = modbusClient->registerRegisterReading(this, beaconMali.slave_address, beaconMali.function_address_cordX);
    beaconMali.modID_cordY = modbusClient->registerRegisterReading(this, beaconMali.slave_address, beaconMali.function_address_cordY);
}

void BeaconDriver::startBeacon(){
    // note:
    // first must be choses number of beacons then beacon shoul be started

    if(beaconConfig.numberOfBeacons == 1) modbusClient->setCoil(beaconConfig.slave_address,beaconConfig.numberOfBeacons_address,0);
    else modbusClient->setCoil(beaconConfig.slave_address, beaconConfig.numberOfBeacons_address,1);

    modbusClient->setCoil(beaconConfig.slave_address,beaconConfig.power_address, 1);

    modbusClient->setReading(beaconMali.modID_cordX, true);
    modbusClient->setReading(beaconMali.modID_cordY, true);

    modbusClient->setReading(beaconVeliki.modID_cordX,true);
    modbusClient->setReading(beaconVeliki.modID_cordY,true);
}

void BeaconDriver::stopBeacon(){

    modbusClient->setCoil(beaconConfig.slave_address,beaconConfig.power_address,0);

    modbusClient->setReading(beaconMali.modID_cordX, false);
    modbusClient->setReading(beaconMali.modID_cordY, false);

    modbusClient->setReading(beaconVeliki.modID_cordX, false);
    modbusClient->setReading(beaconVeliki.modID_cordY, false);
}

void BeaconDriver::callbackRegisterFunction(int _mapID, short _data){

    if(_mapID == beaconVeliki.modID_cordX ) { beaconVeliki.cordX = _data; modbusClient->setReading(beaconVeliki.modID_cordX, true);
        beaconVeliki.settedX = true; }
    else if(_mapID == beaconVeliki.modID_cordY){ beaconVeliki.cordY = _data; modbusClient->setReading(beaconVeliki.modID_cordY, true);
        beaconVeliki.settedY = true; }
    else if(_mapID == beaconMali.modID_cordX){ beaconMali.cordX = _data; modbusClient->setReading(beaconMali.modID_cordX,true);
        beaconMali.settedX = true; }
    else if(_mapID == beaconMali.modID_cordY){ beaconMali.cordY = _data; modbusClient->setReading(beaconMali.modID_cordY,true);}
    else {
        std::cout << "ERRORRR IN BEACON ID" << std::cout;
    }
    if(beaconVeliki.settedX && beaconVeliki.settedY){
        if(beaconVeliki.cordX > 3000 || beaconVeliki.cordY > 2000 ){ beaconVeliki.running = false; }
        else { beaconVeliki.running = true; }
        beaconVeliki.settedX = false;
        beaconVeliki.settedY = false;
        interface->beaconDriverCallbackVeliki(beaconVeliki.cordX, beaconVeliki.cordY,beaconVeliki.running );
    }else if(beaconMali.settedX && beaconMali.settedY){
        if(beaconMali.cordX > 3000 || beaconMali.cordY > 2000){ beaconMali.running = false; }
        else{ beaconMali.running = true; }
        beaconMali.settedX = false;
        beaconMali.settedY = false;
        interface->beaconDriverCallbackMali(beaconMali.cordX, beaconMali.cordY, beaconMali.running);
    }
}

} // end namespace
