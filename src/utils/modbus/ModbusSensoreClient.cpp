#include "ModbusSensoreClient.h"

namespace modbus{

void ModbusSensorClientInterface::ProcessSensorCallback(){
    std::cout<< "called sensore callback, PLEASE REDEFINE" << std::endl;
}
void ModbusSensorClientInterface::ProcessBeackonCallback(){
    std::cout<< "called beckon callback, PLEASE REDEFINE" << std::endl;
}


bool operator< (const idData &id1, const idData &id2){
    if((id1.functionAddress*id1.slaveAddress)< (id2.functionAddress * id2.slaveAddress)) return true;
    else return false;
}


ModbusSensorClient* ModbusSensorClient::instance = 0;

ModbusSensorClient::ModbusSensorClient(): Node("ModbusServoClient"),m_mutex(new boost::mutex()){
    modbus = ModbusClient::getMobusClientInstance();
    start();
    std::cout << "ModbusSensoreClient is runnging" << std::endl;
    //instance = this;
}

ModbusSensorClient* ModbusSensorClient::getModbusSensorInstance(){
    if (!instance){
        instance = new ModbusSensorClient();
    }
    return instance;
}


void ModbusSensorClient::registerToSensoreCallback(unsigned char _slave_address, unsigned short _coil_address, bool on_bool_callback,
                                                   ModbusSensorClientInterface *obj){

    boost::lock_guard<boost::mutex> lock(*m_mutex);

    idData id;
    id.slaveAddress = _slave_address;
    id.functionAddress = _coil_address;
    callbackRegisterMap[id] =(ModbusSensorClientInterface*) obj;
}

void ModbusSensorClient::main(){
    bool success = false;
    bool doCallbak = false;

    while(!shouldStop){
        for(it_type it = callbackRegisterMap.begin(); it != callbackRegisterMap.end(); ++it){
            if(it->second->readingSensore == true){
                success = modbus->readCoil(&doCallbak, it->first);
                if (doCallbak){
                    it->second->ProcessSensorCallback();
                    it->second->readingSensore = false;
                }
            }
            boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
        }
    }
}

void ModbusSensorClient::stopModbusSensorClient(){
    shouldStop = true;
}


} // end namespace modbus
