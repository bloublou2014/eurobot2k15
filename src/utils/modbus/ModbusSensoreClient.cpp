#include "ModbusSensoreClient.h"

namespace modbus{
/*
void ModbusSensorClientInterface::ProcessSensorCallback(){
    std::cout<< "called sensore callback, PLEASE REDEFINE" << std::endl;
}
void ModbusSensorClientInterface::ProcessBeaconCallback(){
    std::cout<< "called beckon callback, PLEASE REDEFINE" << std::endl;
}

void ModbusSensorClientInterface::ProcessEnemySensorCallback1(){
    std::cout<< "called sensore callback, PLEASE REDEFINE" << std::endl;
}

void ModbusSensorClientInterface::ProcessEnemySensorCallback2(){
    std::cout<< "called sensore callback, PLEASE REDEFINE" << std::endl;
}

void ModbusSensorClientInterface::ProcessEnemySensorCallback3(){
    std::cout<< "called sensore callback, PLEASE REDEFINE" << std::endl;
}

void ModbusSensorClientInterface::ProcessEnemySensorCallback4(){
    std::cout<< "called sensore callback, PLEASE REDEFINE" << std::endl;
}
*/
bool operator< (const idData &id1, const idData &id2){
    if((id1.functionAddress*id1.slaveAddress)< (id2.functionAddress * id2.slaveAddress)) return true;
    else return false;
}


ModbusSensorClient* ModbusSensorClient::instance = 0;

ModbusSensorClient::ModbusSensorClient(): Node("ModbusServoClient"),m_mutex(new boost::mutex()){
    modbus = ModbusClient::getMobusClientInstance();
    start();
    std::cout << "ModbusSensoreClient is runnging" << std::endl;
    ModbusSensoreClientNotifier = modbus->getModbusSensorNotifier();
    ModbusSensorClientPanic = modbus->getModbusSensorPanic();
    respinNumber = int(2000/ delayTime);
    brxonID.slaveAddress = char(4);
    brxonID.functionAddress = (char(1));
    brxonStartID.slaveAddress = char(4);
    brxonStartID.functionAddress = char(8);

    beaconID.slaveAddress = char(2);
    beaconID.functionAddress_Start = char(1);
    beaconID.functionAddress_NumberOfBeacons = char(2);
    beaconID.functionAddress_XdataBecaon1 = char(1);
    beaconID.functionAddress_YdataBeacon1 = char(2);
    beaconID.functionAddress_XdataBecaon2 = char(3);
    beaconID.functionAddress_YdataBeacon2 = char(4);
    beaconID.beaconON = false;

    m_mutex->lock();
    *ModbusSensorClientPanic = false;
    m_mutex->unlock();



}

ModbusSensorClient* ModbusSensorClient::getModbusSensorInstance(){
    if (!instance){
        instance = new ModbusSensorClient();
    }
    return instance;
}


void ModbusSensorClient::registerToSensoreCallback(unsigned char _slave_address, unsigned short _coil_address, bool on_bool_callback,
                                                   ModbusSensorClientInterface2 *obj){

    boost::lock_guard<boost::mutex> lock(*m_mutex);

    idData id;
    id.slaveAddress = _slave_address;
    id.functionAddress = _coil_address;
    callbackRegisterMap[id] =(ModbusSensorClientInterface2*) obj;
}

void ModbusSensorClient::main(){
    //bool success = false;
    bool doCallbak = false;
    bool recalculate = false;
    timerForDelayTime = 0;

    while(!shouldStop && !callbackRegisterMap.empty()){

        for(it_type it = callbackRegisterMap.begin(); it != callbackRegisterMap.end(); ++it){

            m_mutex->lock();
            recalculate = *ModbusSensoreClientNotifier;
            m_mutex->unlock();

            if(recalculate && !delayAlreadyCalculated){
                delayAlreadyCalculated = true;
                delayTime = delayTime2;
                std::cout << "sensore Modbus slowing down " << std::endl;
            }

            //std::cout << timerForDelayTime << " : " << respinNumber << std::endl;
            //std::cout << "delayTime" << delayTime << std::endl;enemyDetected

            if(timerForDelayTime < respinNumber && didReading){
                timerForDelayTime ++;
                //std::cout << "counter++ " << std::endl;
            }else{
                // std::cout << "end of counter " << std::endl;
                delayTime = delayTime1;
                delayAlreadyCalculated = false;
                timerForDelayTime = 0;

                m_mutex->lock();
                *ModbusSensoreClientNotifier = false;
                m_mutex->unlock();
                //std::cout << "sensor Modbus speed boosting" << std::endl;

            }

            if(beaconReading > 10 && beaconID.beaconON){
                beaconReading = 0;
                std::cout << " reading beacon" << std::endl;
                readBeacon();
                boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
            }else if(beaconID.beaconON){
                beaconReading++;
            }


            if(it->second->readingSensore == true){
                modbus->readCoil(&doCallbak, it->first);
                //std::cout << "reading coil" << std::endl;
                if (doCallbak){
                    it->second->readingSensore = false;
                    //std::cout << "detected" << std::endl;
                     switch(it->first.functionAddress){
                    case 2: it->second->ProcessEnemySensorCallback1(); it->second->enemyDetected.left = true; break;
                    case 1: it->second->ProcessEnemySensorCallback2(); it->second->enemyDetected.right = true; break;
                    case 3: it->second->ProcessLiftRightSensoreCallback(); std::cout << "desni" << std::endl; break;
                    case 4: it->second->ProcessLiftLeftSensoreCallback(); std::cout << "levi"  << std::endl; break;
                    case 5: it->second->ProcessEnemySensorCallback3(); it->second->enemyDetected.back = true; break;
                    case 7: it->second->ProcessEnemySensorCallback4(); it->second->enemyDetected.brkon = true; break;
                    default: it->second->ProcessSensorCallback();
                    }
                }else{
                    switch(it->first.functionAddress){
                    case 2: it->second->ProcessNotEnemySensorCallback1(); it->second->enemyDetected.left = false;  break;
                    case 1: it->second->ProcessNotEnemySensorCallback2(); it->second->enemyDetected.right = false; break;
                    case 5: it->second->ProcessNotEnemySensorCallback3(); it->second->enemyDetected.back = false; break;
                    case 7: it->second->ProcessNotEnemySensorCallback4(); it->second->enemyDetected.brkon = false; break;
                    }
                }
                didReading = true;

                boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
            }
           boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
        }
    }
    std::cout  << "STOPPING MODBUS SENSORE CLIENT" << std::endl;
}

void ModbusSensorClient::stopModbusSensorClient(){
    shouldStop = true;
}

short ModbusSensorClient::readBrxon(){
    short data;
    bool success;
    success = modbus->readRegister(&data, brxonID.slaveAddress, brxonID.functionAddress);
    return data;
}

bool ModbusSensorClient::startBrxon(){
    modbus->setCoil(brxonStartID.slaveAddress, brxonStartID.functionAddress, 1);
    return true;
}

bool ModbusSensorClient::stopBrxon(){
    modbus->setCoil(brxonStartID.slaveAddress, brxonStartID.functionAddress, 0);
    return true;
}

bool ModbusSensorClient::startBeacon(){
    bool success = false;
    if(!beaconID.beaconON){

        success =  modbus->setCoil(beaconID.slaveAddress, beaconID.functionAddress_NumberOfBeacons, 1 );
        success =  modbus->setCoil(beaconID.slaveAddress, beaconID.functionAddress_Start, 1 );

        if(success){
            beaconID.beaconON = true;
            std::cout << " Beacon is spping " << std::endl;
        }else{
            std::cout << " Beacon ERROR" << std::endl;
        }
    }
    return success;
}

bool ModbusSensorClient::stopBecaon(){
    bool success = false;
    if(beaconID.interface != NULL){
        success =  modbus->setCoil(beaconID.slaveAddress, beaconID.functionAddress_NumberOfBeacons, 0 );
        modbus->setCoil(beaconID.slaveAddress, beaconID.functionAddress_Start, 0);
        if(success) beaconID.beaconON = false;
    }else{
        std::cout<< "have to register to beacon interface firts";
    }
    return false;
}

bool ModbusSensorClient::readBeacon(){
    short data;
    bool success;
    switch (beaconID.beaconDataCounter){
    case 0: {
        success = modbus->readRegister(&data ,beaconID.slaveAddress, beaconID.functionAddress_XdataBecaon1);
        if(success){
            beaconID.beaconData.X_beacon1 = data;
            beaconID.beaconDataCounter++;
        }
        //break;
    }
    case 1:{
        success = modbus->readRegister(&data ,beaconID.slaveAddress, beaconID.functionAddress_YdataBeacon1);
        if(success){
            beaconID.beaconData.Y_beacon1 = data;
            beaconID.beaconDataCounter++;
        }
        //break;
    }
    case 2:{
        success = modbus->readRegister(&data ,beaconID.slaveAddress, beaconID.functionAddress_XdataBecaon2);
        if(success){
            beaconID.beaconData.X_beacon1 = data;
            beaconID.beaconDataCounter++;
        }
        //break;
    }
    case 3:{
        success = modbus->readRegister(&data ,beaconID.slaveAddress, beaconID.functionAddress_YdataBeacon2);
        if(success){
            beaconID.beaconData.X_beacon1 = data;
            beaconID.beaconDataCounter = 0;
            beaconID.interface->beaconData.X_beacon1 = beaconID.beaconData.X_beacon1;
            beaconID.interface->beaconData.Y_beacon1 = beaconID.beaconData.Y_beacon1;
            beaconID.interface->beaconData.X_beacon2 = beaconID.beaconData.X_beacon2;
            beaconID.interface->beaconData.Y_beacon1 = beaconID.beaconData.Y_beacon2;
            beaconID.interface->ProcessBeaconCallback();
            std::cout << "BEACON WRITE DATA" << std::endl;
        }
        //break;
    }
    } // end switch

    return success;
}

void ModbusSensorClient::registerToBeaconInterface(ModbusSensorClientInterface2 *_interface){
    std::cout << "registred to Beacon Interface" << std::endl;
    beaconID.interface = _interface;
}

} // end namespace modbus
