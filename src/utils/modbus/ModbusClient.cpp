#include "ModbusClient.h"

namespace modbus{

void ModbusClientCallbackInterface::modbusEventReceived(){

}

ModbusClient* ModbusClient::instance=0;

ModbusClient::ModbusClient():Node("ModbusClient"),m_mutex(new boost::mutex()){
    modbus = ModbusMaster::getModbusInstance();
    std::cout << "ModbusClient is running" << std::endl;
    instance = this;
    start();
}

ModbusClient* ModbusClient::getMobusClientInstance(){
    if(!instance){
        instance = new modbus::ModbusClient();
    }
    return instance;
}


void ModbusClient::main(){

    while(!shouldStop){

        if(!registersToSet.empty()){
            writeToRegister();
        }

    }
}



bool ModbusClient::setRegister(unsigned char _slave_address, short _function_address, short _data ){ // za klienta
    setSingleRegisterData data;

    data.ID.slaveAddress = _slave_address;
    data.ID.functionAddress = _function_address;
    data.data = _data;

    queueLock.lock();
    registersToSet.push(data);
    queueLock.unlock();

    return true;
}


bool ModbusClient::writeToRegister(){
    bool success;
    setSingleRegisterData data;

    queueLock.lock();
    if(!registersToSet.empty()){
        data =(setSingleRegisterData) registersToSet.front();
        registersToSet.pop();
        queueLock.unlock();
    }else{
        queueLock.unlock();
        return true;
    }

    std::cout << "writeing to register: "
              << int(data.ID.slaveAddress) << ":"
              << data.ID.functionAddress << ":"
              << data.data << std::endl;

    boost::lock_guard<boost::mutex> lock(*m_mutex);

    success = modbus->ModbusPresetSingleRegister(data.ID.slaveAddress, data.ID.functionAddress, data.data);
    if (!success){
        boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
        success = modbus->ModbusPresetSingleRegister(data.ID.slaveAddress, data.ID.functionAddress, data.data);
    }
    return success;
}

bool ModbusClient::readCoil(bool* _callFunction, idData _id ){
    signed char data;
    boost::lock_guard<boost::mutex> lock(*m_mutex);

    bool success = modbus->ModbusReadCoilStatus(_id.slaveAddress,_id.functionAddress,1, &data);

    if (!success){  // try 2  if false return false
        boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));

        success = modbus->ModbusReadCoilStatus(_id.slaveAddress,_id.functionAddress,1, &data);
    }
/*
    std::cout << "reading from register: "
              << int(_id.slaveAddress) << " : "
              << _id.functionAddress << " = "
              << int(data) << " : "
              << data
              << std::endl;
*/
    if (!success){
        *_callFunction = false;
        return false;
    }

    if(data == char(1)){
        *_callFunction = true;
        //std::cout << "OBJECT" << std::endl;
    }else if (data == '1'){
        *_callFunction = true;
        //std::cout << "OBJECT" << std::endl;
    }else if(data == char(0)){
        *_callFunction = false;
    }else if(data == '0'){
        *_callFunction = false;
    }else{
        std::cout << "ERRORO IN CHECKING STATE" << std::endl;
        *_callFunction = false;
    }

    return true;
}

void ModbusClient::stopModbusClient(){
    shouldStop = true;
}



} // end namespace
