#ifndef _SENSORS_H
#define _SENSORS_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include <stdio.h>
#include <stdlib.h>

#include "utils/modbus/ModbusMaster.h"



using namespace modbus;
using modbus::ModbusMaster;

namespace sensor {

class SensorDriver{
private:
    unsigned char slave_address;
    unsigned short scan_address;
    unsigned short enemy_position_address;
    bool proximity;

    boost::mutex *io_mutex;
    ModbusMaster *modbus;

public:
    SensorDriver();
    SensorDriver(unsigned char _slave_address, unsigned short _scan_address, unsigned short _enemy_position_address);
    ~SensorDriver();

    void setSlaveAddress(unsigned char _slave_address);
    void setScanAddress(unsigned short _scan_address);
    void setEnemyPositionAddress(unsigned short _enemy_position_address);
    void setProximitySensore(bool _proxymity);

    bool scanSensorStatus();  // returns True if sensro is scaneing something in his rangde
    int getEnemyPosition();  // returns int angle where 0 is defined by slave microcontroler
};

} // end namespace sensor


#endif // _SENSORS_H
