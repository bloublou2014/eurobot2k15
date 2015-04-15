#include "EnemyDetector.h"

namespace executor{

std::string EnemyDetector::NAME = "EnemyDetector";

void EnemyDetector::suscribe(){
    modbusClient = ModbusSensorClient::getModbusSensorInstance();
    modbusClient->registerToSensoreCallback(char(4), char(1),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(2),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(5),true,this);

}

void EnemyDetector::ProcessSensorCallback(){
    testBool = true;
}

bool EnemyDetector::liftLoop(){

    if(testBool){
        std::cout << "ENEMY DETECTED" << std::endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
        readingSensore = true;
    }
}

}
