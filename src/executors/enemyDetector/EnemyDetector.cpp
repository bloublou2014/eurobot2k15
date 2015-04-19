#include "EnemyDetector.h"

namespace executor{

std::string EnemyDetector::NAME = "EnemyDetectorExecutor";


void EnemyDetector::suscribe(){

    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&EnemyDetector::processActuatorCommand));
    modbusClient = ModbusSensorClient::getModbusSensorInstance();
    modbusClient->registerToSensoreCallback(char(4), char(1),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(2),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(5),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(7),true,this);
    modbusClient->registerToBeaconInterface(this);
    this->readingSensore = true;
    //modbusClient->startBeacon();
    //modbusClient->startBrxon();

    previousState.back=false;
    previousState.brkon=false;
    previousState.left=false;
    previousState.right=false;
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

void EnemyDetector::ProcessSensorCallback(){
    testBool = true;
    std::cout << "EMENY" << std::endl;
}

void EnemyDetector::ProcessEnemySensorCallback1(){
    testBool = true;
    if (previousState.left!=true){
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::SENSOR, 0);
        sendNotification(notification);
        previousState.left=true;
    }
}

void EnemyDetector::ProcessEnemySensorCallback2(){
    testBool = true;
    if (previousState.right!=true){
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::SENSOR, 0);
        sendNotification(notification);
        previousState.right=true;
    }
}

void EnemyDetector::ProcessEnemySensorCallback3(){
    testBool = true;
    if (previousState.back!=true){
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::SENSOR, 180);
        sendNotification(notification);
        previousState.back=true;
    }
}

void EnemyDetector::ProcessEnemySensorCallback4(){
    //short data;
    int angle;
    testBool = true;
    if (previousState.brkon!=true){
        angle = modbusClient->readBrxon()-85;   //85 zato sto je tako
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BRKON,angle);
        sendNotification(notification);
        previousState.brkon=true;
    }
}

void EnemyDetector::ProcessBeaconCallback(){
    testBool = true;
    std::cout << "BECON" << std::endl;
    std::cout << this->beaconData.X_beacon1 << std::endl
              << this->beaconData.Y_beacon1 << std::endl
              << this->beaconData.X_beacon2 << std::endl
              << this->beaconData.Y_beacon2 << std::endl;
}

bool EnemyDetector::liftLoop(){
    //std::cout << "redone" << std::endl;
    if(testBool){
        testBool = false;
        //std::cout << "ENEMY DETECTED" << std::endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(20));
        this->readingSensore = true;
        if (previousState.brkon && !enemyDetected.brkon){
            EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BRKON, 0, false);
            sendNotification(notification);
            previousState.brkon=false;
        }
        if (previousState.back && !enemyDetected.back){
            EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::SENSOR, 180, false);
            sendNotification(notification);
            previousState.back=false;
        }
        if (previousState.left && !enemyDetected.left){
            EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BRKON, 0, false);
            sendNotification(notification);
            previousState.left=false;
        }
        if (previousState.right && !enemyDetected.right){
            EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BRKON, 0, false);
            sendNotification(notification);
            previousState.right=false;
        }
    }
    return true;
}

}
