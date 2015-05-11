#include "EnemyDetector.h"

namespace executor{

std::string EnemyDetector::NAME = "EnemyDetectorExecutor";


void EnemyDetector::suscribe(){

    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&EnemyDetector::processActuatorCommand));
    modbusClient = ModbusSensorClient::getModbusSensorInstance();
#ifdef VELIKI_ROBOT
    modbusClient->registerToSensoreCallback(char(4), char(1),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(2),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(5),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(7),true,this);
    modbusClient->registerToBeaconInterface(this);
    this->readingSensore = true;
#endif

#ifdef MALI_ROBOT
    modbusClient->registerToSensoreCallback(char(1), char(1),true,this);
    modbusClient->registerToSensoreCallback(char(1), char(2),true,this);
    modbusClient->registerToSensoreCallback(char(1), char(3),true,this);
    this->readingSensore = true;
#endif

    //modbusClient->startBeacon();
    //modbusClient->startBrxon();

    previousState.back=false;
    previousState.brkon=false;
    previousState.left=false;
    previousState.right=false;
    previousBackBrkon=0xFF;     //Empty
    previousFrontBrkon=0xFF;    //Empty
}

bool EnemyDetector::StartBeaconFunction(){
    debug("Start Beacon");
    bool success;
    success = modbusClient->startBeacon();
    return success;
}

bool EnemyDetector::StopBeaconFunction(){
    debug("Stop Beacon");
    bool success;
    success = modbusClient->stopBecaon();
    return success;
}

bool EnemyDetector::StartBrxonFunction(){
    debug("Start Brxon");
    bool success;
    success = modbusClient->startBrxon();
    return success;
}

bool EnemyDetector::StopBrxonFunction(){
    debug("Stop Brxon");
    bool success;
    success = modbusClient->stopBecaon();
    return success;
}

bool EnemyDetector::StopDetectionFunction(){
    debug("STOPING SENSORES");
    readingSensore = false;
    return true;
}

bool EnemyDetector::StartDetectionFunction(){
    debug("START DETECTION");
    readingSensore = true;
    return  true;
}

void EnemyDetector::ProcessSensorCallback(){
    testBool = true;
    std::cout << "EMENY" << std::endl;
}

void EnemyDetector::ProcessEnemySensorCallback1(){
    readingSensore = true;
}

void EnemyDetector::ProcessEnemySensorCallback2(){
    readingSensore = true;

}

void EnemyDetector::ProcessEnemySensorCallback3(){
    readingSensore = true;

    testBool = true;
    if (previousState.back!=true){
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BACK, 180);
        sendNotification(notification);
        previousState.back=true;
    }
}

void EnemyDetector::ProcessEnemySensorCallback4(){

}

void EnemyDetector::ProcessBeaconCallback(){
    readingSensore = true;

    testBool = true;
    std::cout << "BECON" << std::endl;
    std::cout << this->beaconData.X_beacon1 << std::endl
              << this->beaconData.Y_beacon1 << std::endl
              << this->beaconData.X_beacon2 << std::endl
              << this->beaconData.Y_beacon2 << std::endl;
}

void EnemyDetector::ProcessNotEnemySensorCallback1(){

}

void EnemyDetector::ProcessNotEnemySensorCallback2(){

}

void EnemyDetector::ProcessNotEnemySensorCallback3(){
    if (previousState.back){
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BACK, 180, false);
        sendNotification(notification);
        previousState.back=false;
    }
}

void EnemyDetector::ProcessNotEnemySensorCallback4(){

}


}
