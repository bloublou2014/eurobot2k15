#include "SensorDriver.h"


namespace sensor{

SensorDriver::SensorDriver(){
    modbusClient = ModbusClientSW::getModbusClientInstance();
}

SensorDriver::SensorDriver(unsigned char _slave_address, short _function_address, int _ID, SensorDriverInterface* _interface, bool _callbackOnNotDetected){
     modbusClient = ModbusClientSW::getModbusClientInstance();
     slave_address = _slave_address;
     function_address = _function_address;
     sensorID = _ID;
     interface = _interface;
     callbackOnNotDetected = _callbackOnNotDetected;
}

void SensorDriver::RegisterSensor(){
    modbusID = modbusClient->registerCoilReading(this, slave_address, function_address, callbackOnNotDetected );
}

void SensorDriver::callbackCoilFunction(int _Id, bool _detected){
    // when sensre detects something modbusClient will call this function;
    interface->SensorDriverCallback(sensorID, _detected);

}

void SensorDriver::StartSensor(){
    modbusClient->setReading(modbusID, true);
}

void SensorDriver::StopSensor(){
    modbusClient->setReading(modbusID, false);
}

void SensorDriver::setConfig(unsigned char _slave_address, short _function_address, int _ID, SensorDriverInterface* _interface, bool _callbackOnNotDetected){
    slave_address = _slave_address;
    function_address = _function_address;
    sensorID = _ID;
    interface = _interface;
    callbackOnNotDetected = _callbackOnNotDetected;
}

} // end namespace sensor
