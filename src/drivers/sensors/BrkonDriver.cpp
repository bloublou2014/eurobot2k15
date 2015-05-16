#include "BrkonDriver.h"

namespace sensor{

void BrkonDriver::setCoilConfig(unsigned char _slave_address, short _function_address){
    coilID.slave_address = _slave_address;
    coilID.function_address = _function_address;
}

void BrkonDriver::setRegisterConfig(unsigned char _slave_address, short _function_address){
    registerID.function_address = _function_address;
    registerID.slave_address = _slave_address;
}

void BrkonDriver::setPowerCoilConfig(unsigned char _slave_address, short _function_address){
    powerID.function_address = _function_address;
    powerID.slave_address = _slave_address;
}

void BrkonDriver::startBrkon(){
    std::cout << "BRKON DRIVER [D] Starting Brkon" << std::endl;
    modbusClient->setReading(coilID.modID, true);
    modbusClient->setCoil(powerID.slave_address, powerID.function_address,1,false);
    return;

}

void BrkonDriver::stopBrkon(){
    modbusClient->setReading(coilID.modID, false);
    modbusClient->setReading(registerID.modID, false);
    modbusClient->setCoil(powerID.slave_address, powerID.function_address,0,false);

    modbusClient->setReading(coilID.modID, false);
    modbusClient->setReading(registerID.modID, false);
    modbusClient->setCoil(powerID.slave_address, powerID.function_address,0,false);

    modbusClient->setReading(coilID.modID, false);
    modbusClient->setReading(registerID.modID, false);
    modbusClient->setCoil(powerID.slave_address, powerID.function_address,0,false);

}

void BrkonDriver::registerBrkon(){
    coilID.modID = modbusClient->registerCoilReading(this, coilID.slave_address, coilID.function_address,true);
    registerID.modID = modbusClient->registerRegisterReading(this, registerID.slave_address, registerID.function_address);
}

void BrkonDriver::registerInerface(BrkonDriverInterface* _callback_interface){
    callbakInterface =_callback_interface;
}

void BrkonDriver::callbackCoilFunction(int _mapID, bool _detected){
    if(_detected){
        //std::cout << "BRKON DRIVER [D] callback" << std::endl;
        if (_mapID == coilID.modID){
            modbusClient->setReading(registerID.modID, true);
            modbusClient->setReading(coilID.modID, true);
        }else{
            std::cout << "ERRRORRRR BRKON DRIVER" << std::endl;
        }
    }else{
        callbakInterface->brkonDriverCallback(0xFF,0xFF, false);
        modbusClient->setReading(coilID.modID, true);


    }

}

void BrkonDriver::callbackRegisterFunction(int _mapID, short _data){
    if(_mapID == registerID.modID){
        unsigned short dataFront = _data;
        unsigned short dataBack = _data;
        //printf("%X \n", _data);
        dataFront = dataFront & 0xFF00;
        dataFront = dataFront >> 8;
        dataBack = dataBack & 0x00FF;

        callbakInterface->brkonDriverCallback(dataFront, dataBack, true);
    }
}

} // end namespace
