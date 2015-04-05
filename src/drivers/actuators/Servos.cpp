#include "Servos.h"
#include <stdio.h>
#include <stdlib.h>

using namespace modbus;
using modbus::ModbusMaster;

namespace servo {


ServoDriver::ServoDriver():s_mutex(new boost::mutex()){
    //this->robot = VELIKI;
    this->speed_address = 'b';
    this->position = 0; 
    modbus = ModbusMaster::getModbusInstance();


}

ServoDriver::ServoDriver(unsigned char slave_address = 'a' , unsigned short position_address = 'a', unsigned short speed_address = 'a' ):s_mutex(new boost::mutex()){
    //this->robot = robotType;
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
    printf("ServoDriver: rotateFor\n,");
    short position_calc;
    bool success = true;
    position_calc = this->position + steps;

    if ((position_calc >= 0 ) && (position_calc <= 1023)){

        success = modbus->ModbusPresetSingleRegister(this->slave_address, this->position_address,position_calc);

    }else{
        printf("ERROR: POSITION OUT OF VALUE");
        return false;

    }

    return success;
}

bool ServoDriver::rotateToPosition(short position){

    boost::lock_guard<boost::mutex> lock(*s_mutex);
    printf("ServoDriver: rotateTo %d, %d \n", position, this->position_address);
    bool success = true;

    if ((position >= 0 ) && (position <= 1023)){
        success = modbus->ModbusPresetSingleRegister(this->slave_address, this->position_address, position);
    }else{
        printf("ERROR: POSITION OUT OF VALUE");
        return false;
    }
     printf("sucess %d \n ", success);
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

void ServoDriver::setServoStatusSetAddress(unsigned short _address){
    this->statusSet_address = _address;
}

void ServoDriver::setServoStatusReadAddress(unsigned short _address){
    this->statusRead_address = _address;
}


bool ServoDriver::getServoStatus(){
    bool status;
    signed char result_char;
    boost::lock_guard<boost::mutex> lock(*s_mutex);
    modbus->ModbusForceSingleCoil(slave_address,statusSet_address, char(1));
    modbus->ModbusReadCoilStatus(slave_address,statusRead_address, 1, &result_char);
    printf("return value of coil is %d /n", result_char);

    if ( result_char == char(1)){
        status = true;
    }else if ( result_char == char(0)){
        status = false;
    }else if( result_char == '1'){
        status = true;
    }else if( result_char == '0'){
        status = false;
    }else{
        printf("ERROR: DRIVER NO COIL STATUS");
    }
    return status;
}


} // end namespace