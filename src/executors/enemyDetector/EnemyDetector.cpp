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
    modbusClient->registerToSensoreCallback(char(4), char(1),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(1),true,this);
    modbusClient->registerToSensoreCallback(char(4), char(1),true,this);
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

void EnemyDetector::ProcessSensorCallback(){
    testBool = true;
    std::cout << "EMENY" << std::endl;
}

void EnemyDetector::ProcessEnemySensorCallback1(){
    readingSensore = true;
    testBool = true;
    if (previousState.left!=true){
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::LEFT, 0);
        sendNotification(notification);
        previousState.left=true;
    }
}

void EnemyDetector::ProcessEnemySensorCallback2(){
    readingSensore = true;

    testBool = true;
    if (previousState.right!=true){
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::RIGHT, 0);
        sendNotification(notification);
        previousState.right=true;
    }
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
    readingSensore = true;
    testBool = true;

    using namespace std;


    unsigned short data=modbusClient->readBrxon();
//        printf("/nreceived brkon notification: %x %d\n",data, sizeof(data));
    unsigned char back=(char)data&0x00FF;
    unsigned char front=(char)(data>>2)&0x00FF;

    //Provera zadnjeg
    if (!(back^0xFF)){  //Ako nije detektovano nista nazad
        if (previousBackBrkon!=back){   //Ako je nesto bilo detektovano u prethodnoj iteraciji posalji da ga vise nema
            cout<<"Sending back brkon"<<endl;
            EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BRKON_BACK, previousBackBrkon, false);
            sendNotification(notification);
        }
    }else{  //Ako je nesto detektovano
        if (previousBackBrkon==0xFF){   //a malopre nije nista, posalji notifikaciju da se nesto pojavilo
            cout<<"Sending front brkon"<<endl;
            EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BRKON_BACK,back);
            sendNotification(notification);
        }
    }
    previousBackBrkon=back;

    //Provera prednjeg
    if (!(front^0xFF)){  //Ako nije detektovano nista nazad
        if (previousFrontBrkon!=front){   //Ako je nesto bilo detektovano u prethodnoj iteraciji posalji da ga vise nema
            EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BRKON_FRONT, previousFrontBrkon, false);
            sendNotification(notification);
        }
    }else{  //Ako je nesto detektovano
        if (previousFrontBrkon==0xFF){   //a malopre nije nista, posalji notifikaciju da se nesto pojavilo
            EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BRKON_FRONT,front);
            sendNotification(notification);
        }
    }
    previousFrontBrkon=front;
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
    if (previousState.left){
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::LEFT, 0, false);
        sendNotification(notification);
        previousState.left=false;
    }
}

void EnemyDetector::ProcessNotEnemySensorCallback2(){
    if (previousState.right){
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::RIGHT, 0, false);
        sendNotification(notification);
        previousState.right=false;
    }
}

void EnemyDetector::ProcessNotEnemySensorCallback3(){
    if (previousState.back){
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BACK, 180, false);
        sendNotification(notification);
        previousState.back=false;
    }
}

void EnemyDetector::ProcessNotEnemySensorCallback4(){
    if (previousBackBrkon^0xFF){ //Ako je nesto bilo setovano za zadnji
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BRKON_BACK, previousBackBrkon, false);
        sendNotification(notification);
        previousBackBrkon=0xFF;
    }
    if (previousFrontBrkon^0xFF){ //Ako je nesto bilo setovano za zadnji
        EnemyDetectedNotification* notification=new EnemyDetectedNotification(EnemyDetectedNotification::BRKON_FRONT, previousFrontBrkon, false);
        sendNotification(notification);
        previousFrontBrkon=0xFF;
    }
}


}
