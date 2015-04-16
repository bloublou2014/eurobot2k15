#include "EnemyDetector.h"

namespace executor{

std::string EnemyDetector::NAME = "EnemyDetector";

void EnemyDetector::suscribe(){
    modbusClient = ModbusSensorClient::getModbusSensorInstance();
    modbusClient->registerToSensoreCallback(char(4), char(1),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(2),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(5),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(7),true,this);
    modbusClient->registerToBeaconInterface(this);
    this->readingSensore = true;
    modbusClient->startBeacon();

}

void EnemyDetector::ProcessSensorCallback(){
    testBool = true;
    std::cout << "EMENY" << std::endl;
}

void EnemyDetector::ProcessEnemySensorCallback1(){
    testBool = true;
    std::cout << "ENEMY 1" << std::endl;
}

void EnemyDetector::ProcessEnemySensorCallback2(){
    testBool = true;
    std::cout << "ENEMY 2" << std::endl;
}

void EnemyDetector::ProcessEnemySensorCallback3(){
    testBool = true;
    std::cout << "ENEMY 3" << std::endl;
}

void EnemyDetector::ProcessEnemySensorCallback4(){
    short data;
    testBool = true;
    std::cout << "ENEMY BRXON" << std::endl;
    data = modbusClient->readBrxon();
    std::cout << data << std::endl;
}


void EnemyDetector::ProcessBeaconCallback(){
    testBool = true;
    std::cout << this->beaconData.X_beacon1 << std::endl
              << this->beaconData.Y_beacon1 << std::endl
              << this->beaconData.X_beacon2 << std::endl
              << this->beaconData.Y_beacon2 << std::endl;
}



bool EnemyDetector::liftLoop(){
    //std::cout << "redone" << std::endl;
    if(testBool){
        testBool = false;
        std::cout << "ENEMY DETECTED" << std::endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
        this->readingSensore = true;
    }
    return true;
}

}
