#ifndef _SERVOS_H
#define _SERVOS_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include <stdio.h>
#include <stdlib.h>

#include "utils/modbus/ModbusMaster.h"

using namespace modbus;
using modbus::ModbusMaster;

namespace servo {

class ServoDriver{
public:
    //enum RobotType{MALI , VELIKI };
    enum State{MOVING, ERROR, IDLE };

private:

    //RobotType robot;
    int speed;
    int position;
    bool working;
    unsigned char slave_address;
    unsigned short position_address;
    unsigned short speed_address;
    unsigned short statusSet_address;
    unsigned short statusRead_address;
    signed char dataON;
    signed char dataOFF;

    boost::mutex *s_mutex;
    ModbusMaster *modbus;
    //static ModbusMaster modbus
    //ModbusMaster modbus;

public:



    ServoDriver();
    ServoDriver(unsigned char, unsigned short, unsigned short);
    ~ServoDriver();
    bool rotateFor(short steps);
    bool rotateToPosition(short position);
    void setON(bool);

    void setServoSpeed(int speed);
    void setServoSpeedAddress(unsigned short);
    void setServoPositionAddress(unsigned short);
    void setServoStatusReadAddress(unsigned short _address);
    void setServoStatusSetAddress(unsigned short _address);
    int getServoSpeed();
    int getServoPosition();
    bool getServoStatus();

    void setServoPosition(int position);
    void setServoSlaveAddress(unsigned char);





};


}




#endif  //_SERVOS_H
