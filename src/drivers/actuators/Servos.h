#ifndef _MOVESOMETHING_H
#define _MOVESOMETHING_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include "utils/uart/UartConnection.h"
#include "utils/geometry/Point2D.h"

#include "utils/modbus/ModbusMaster.h"
//#include "utils/modbus/ModbusSingleton.h"

using namespace modbus;
using modbus::ModbusMaster;

namespace servo {

class ServoDriver{
public:
    enum RobotType{MALI , VELIKI };
    enum State{MOVING, STUCK, ERROR, IDLE };

private:

    RobotType robot;
    int speed;
    int position;
    bool working;
    unsigned char slave_address;
    unsigned short position_address;
    unsigned short speed_address;
    signed char dataON;
    signed char dataOFF;

    boost::mutex *s_mutex;
    ModbusMaster *modbus;
    //static ModbusMaster modbus
    //ModbusMaster modbus;

public:



    ServoDriver();
    ServoDriver(RobotType, unsigned char, unsigned short, unsigned short);
    ~ServoDriver();
    bool rotateFor(short steps);
    bool rotateToPosition(short position);
    void setON(bool);

    void setServoSpeed(int speed);
    void setServoSpeedAddress(unsigned short);
    void setServoPositionAddress(unsigned short);
    RobotType getRobotType();
    int getServoSpeed();
    int getServoPosition();

    void setServoPosition(int position);
    void setServoSlaveAddress(unsigned char);
    void setRoboType(RobotType);






};


}




#endif
