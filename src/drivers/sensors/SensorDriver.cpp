#include "SensorDriver.h"


namespace sensor{

SensorDriver::SensorDriver():io_mutex(new boost::mutex){
    modbus = ModbusMaster::getModbusInstance();
    this->slave_address = char(1);
    this->scan_address = char(1);
    this->enemy_position_address = char(1);
    this->proximity = false;
}

SensorDriver::SensorDriver(unsigned char _slave_address, unsigned short _scan_address, unsigned short _enemy_position_address){
    modbus = ModbusMaster::getModbusInstance();
    this->slave_address = _slave_address;
    this->scan_address = _scan_address;
    this->enemy_position_address = _enemy_position_address;
    this->proximity = false;


}

SensorDriver::~SensorDriver(){
    delete io_mutex;
}

void SensorDriver::setSlaveAddress(unsigned char _slave_address){
    this->slave_address = char(_slave_address);
}

void SensorDriver::setScanAddress(unsigned short _scan_address){
    this->scan_address = char(_scan_address);
}

void SensorDriver::setEnemyPositionAddress(unsigned short _enemy_position_address){
    this->enemy_position_address = char(_enemy_position_address);
}

void SensorDriver::setProximitySensore(bool _proxymity){
    this->proximity = _proxymity;
}

bool SensorDriver::scanSensorStatus(){
    boost::lock_guard<boost::mutex> lock(*io_mutex);
    return true;
    // TODO jos ne znam kako ce izgledati samo citanje statusa nekog senzora :(

}


int SensorDriver::getEnemyPosition(){
    if (proximity){
        boost::lock_guard<boost::mutex> lock(*io_mutex);
        return 10 ;

        // TODO moram se naci sa pedjom i za ovo da vidim :)

    }
    return 10 ;

}


} // end namespace sensor
