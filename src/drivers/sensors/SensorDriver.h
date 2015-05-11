#ifndef _SENSORS_H
#define _SENSORS_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include <stdio.h>
#include <stdlib.h>


#include "utils/modbus/ModbusClientSwitzerland.h"

using namespace modbus;
using modbus::ModbusMaster;
using modbus::ModbusClientSW;

namespace sensor{

class SensorDriverInterface{
public:
    virtual void SensorDriverCallback(int _id, bool _detected){ return;}
};

class SensorDriver : public ModbusClientSWInterface, public SensorDriverInterface{

public:
    SensorDriver();
    SensorDriver(unsigned char _slave_address, short _function_address, int _ID, SensorDriverInterface* _interface,bool _callbackOnNotDetected);
    void setConfig(unsigned char _slave_address, short _function_address, int _ID, SensorDriverInterface* _interface, bool _callbackOnNotDetected);
    void RegisterSensor();
    void StartSensor();
    void StopSensor();
private:
    ModbusClientSW* modbusClient;
    int sensorID;
    int modbusID;
    unsigned char slave_address;
    short function_address;
    bool detected;
    bool reading;
    bool callbackOnNotDetected;
    void callbackCoilFunction(int _Id, bool _detected);
    SensorDriverInterface* interface;
};

} // end namespace sensor


#endif // _SENSORS_H
