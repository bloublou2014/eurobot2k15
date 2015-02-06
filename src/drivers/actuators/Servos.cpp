#include "Servos.h"
#include <stdio.h>
#include <stdlib.h>

using namespace modbus;
using modbus::ModbusMaster;

namespace servo {


ServoDriver::ServoDriver():s_mutex(new boost::mutex()){
    this->robot = VELIKI;    
    this->speed_address = 'b';
    this->position = 0; 
    modbus = ModbusMaster::getModbusInstance();


}

ServoDriver::ServoDriver(RobotType robotType = VELIKI , unsigned char slave_address = 'a' , unsigned short position_address = 'a', unsigned short speed_address = 'a' ):s_mutex(new boost::mutex()){
    this->robot = robotType;
    this->slave_address = slave_address;
    this->position_address = position_address;
    this->speed_address = speed_address;
    modbus = ModbusMaster::getModbusInstance();

}

ServoDriver::~ServoDriver(){
    delete s_mutex;
}

bool ServoDriver::rotateFor(short steps){

    boost::lock_guard<boost::mutex> lock(*s_mutex);
    printf("ServoDriver: rotateFor\n");
    short position_calc;
    bool success = true;
    position_calc = this->position + steps;

    if ((position_calc >= 0 ) && (position_calc <= 600)){

        success = modbus->ModbusPresetSingleRegister(this->slave_address, this->position_address,position_calc);

    }
    return success;
}

bool ServoDriver::rotateToPosition(short position){

    boost::lock_guard<boost::mutex> lock(*s_mutex);
    printf("ServoDriver: rotateTo\n");
    bool success = true;

    if ((position >= 0 ) && (position <= 600)){
        success = modbus->ModbusPresetSingleRegister(this->slave_address, this->position_address, position);
    }
    return success;
}

void ServoDriver::setServoSpeed(int speed){

    boost::lock_guard<boost::mutex> lock(*s_mutex);

    if (this->speed != speed ){
        this->speed = speed;

        modbus->ModbusPresetSingleRegister(slave_address, speed_address, speed);
    }
}

void ServoDriver::setServoPosition(int position){
    this->position = position;
}
int ServoDriver::getServoSpeed(){
    return speed;
}

int ServoDriver::getServoPosition(){
    return position;
}

void ServoDriver::setServoSlaveAddress(unsigned char slave_address){
    this->slave_address = slave_address;
}

void ServoDriver::setServoSpeedAddress(unsigned short address){
    this->speed_address = address;
}

void ServoDriver::setServoPositionAddress(unsigned short address ){
    this->position_address = address;

}




}
