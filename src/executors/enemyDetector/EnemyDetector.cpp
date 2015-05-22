#include "EnemyDetector.h"

namespace executor{

std::string EnemyDetector::NAME = "EnemyDetectorExecutor";


void EnemyDetector::suscribe(){

    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&EnemyDetector::processActuatorCommand));

#ifdef VELIKI_ROBOT
    //    frontSensor.setConfig(char(4),char(1),sensorFrontID,this, true);
    //    frontSensor.RegisterSensor();
    //    frontSensor.StartSensor();

    frontLefttSensor.setConfig(char(4),char(1),frontLeftSensorID,this, true);
    frontLefttSensor.RegisterSensor();
//    frontLefttSensor.StartSensor();

    frontRightSensor.setConfig(char(4),char(2),frontRightSensorID,this, true);
    frontRightSensor.RegisterSensor();
//    frontRightSensor.StartSensor();

    backSensor.setConfig(char(4), char(5),sensorBackID, this, true);
    backSensor.RegisterSensor();
//    backSensor.StartSensor();

    brkon.setCoilConfig(char(4),char(7));
    brkon.setRegisterConfig(char(4),char(1));
    brkon.setPowerCoilConfig(char(4),char(8));
    brkon.registerInerface(this);
    brkon.registerBrkon();
    //    brkon.startBrkon();

    //TODO
    //dodati za beacon
    //    beacon.setBeaconConfig(char(2),char(1),char(2),char(2),char(3),char(4),char(2),char(1),char(2),2,this);
    //    beacon.registerBeacon();
    //    beacon.startBeacon();


#endif

#ifdef MALI_ROBOT

    frontLefttSensor.setConfig(char(4),char(1),frontLeftSensorID,this, true);
    frontLefttSensor.RegisterSensor();
    frontLefttSensor.StartSensor();

    frontRightSensor.setConfig(char(4),char(2),frontRightSensorID,this, true);
    frontRightSensor.RegisterSensor();
    frontRightSensor.StartSensor();

    backSensor.setConfig(char(4), char(3),backSesnorID, this, true);
    backSensor.RegisterSensor();
    backSensor.StartSensor();

#endif

}

bool EnemyDetector::StartBrxonFunction(){
    debug("Starting Brkon");
    brkon.startBrkon();
    return true;
}

bool EnemyDetector::StopBrxonFunction(){
    debug("stopBrkon");
    brkon.stopBrkon();
    return true;
}

void EnemyDetector::SensorDriverCallback(int _id, bool _detected){
    EnemyDetectedNotification* notification=NULL;
    //debug("SENSOR CALLBACK ");
    if(_id == this->sensorBackID || _id == backSesnorID){
//        if(previousState.detectionSensorBack != _detected){

            if(_detected && !previousState.detectionBrkonBack ){
                previousState.angleBack = 45;
                previousState.detectionSensorBack = true;
                previousState.detectionBrkonBack = true;
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK,180,true);
                debug("DOSO BACK");
            }else if(!_detected && !previousState.detectionBrkonBack && previousState.detectionSensorBack) {
                previousState.angleBack = 255;

                previousState.detectionSensorBack = false;
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK,180,false);
                debug("OTISAO BACK");
            }
//        }
#ifdef VELIKI_ROBOT
        backSensor.StartSensor();
#endif
#ifdef MALI_ROBOT
        backSensor.StartSensor();
#endif
        /*
    }else if(_id ==this->sensorFrontID || (_id == frontLeftSensorID) || ( _id == frontRightSensorID )){
        if(previousState.detectionSensorFront != _detected){

            if(_detected && !previousState.detectionBrkonFront){
                previousState.detectionSensorFront = _detected;
                previousState.detectionBrkonFront = true;
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,(previousState.angleFront - 50),true);
                debug("DOSO FRONT");
            }else if(!_detected && !previousState.detectionBrkonFront){
                previousState.detectionSensorFront = _detected;
                previousState.angleFront = 255;
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,0,false);
                debug("OTISAO FRONT");
            }
        }
        */
    }else if(_id == this->frontRightSensorID){
//        if(previousState.detectionSensorRightFront != _detected){

            if(_detected && !previousState.detectionBrkonFront && !previousState.detectionSensorLeftFront){
                previousState.angleFront = 50;
                previousState.detectionSensorRightFront = true;
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,(previousState.angleFront - 50),true);
                debug("DOSO FRONT RIGHT");
            }else if(!_detected && !previousState.detectionBrkonFront && !previousState.detectionSensorLeftFront && previousState.detectionSensorRightFront){
                previousState.detectionSensorRightFront = false;
                previousState.angleFront = 255;
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,0,false);
                debug("OTISAO FRONT RIGHT");
            }
//        }
        frontRightSensor.StartSensor();
    }else if(_id == this->frontLeftSensorID){
//        if(previousState.detectionSensorLeftFront != _detected){
            if(_detected && !previousState.detectionBrkonFront && !previousState.detectionSensorRightFront){
                previousState.angleFront = 50;
                previousState.detectionSensorLeftFront = true;
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,(previousState.angleFront - 50),true);
                debug("DOSO FRONT LEFT");
            }else if(!_detected && !previousState.detectionBrkonFront && !previousState.detectionSensorRightFront && previousState.detectionSensorLeftFront){
                previousState.detectionSensorLeftFront = false;
                previousState.angleFront = 255;
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,0,false);
                debug("OTISAO FRONT LEFT");
            }
//        }
        frontLefttSensor.StartSensor();

#ifdef VELIKI_ROBOT

#endif
#ifdef MALI_ROBOT
        frontLefttSensor.StartSensor();
        frontRightSensor.StartSensor();
#endif
    }else{
        debug("WROOONG ID ");
    }

    if (notification!=NULL){
        debug("Sending enemy detected notification!");
        sendNotification(notification);
    }

}

void EnemyDetector::brkonDriverCallback(unsigned char _dataFront, unsigned char _dataBack, bool _detected){
    EnemyDetectedNotification* notification=NULL;
    /*
    std::cout << "angle front: " << _dataFront << std::endl
              << "angle back: " << _dataBack << std::endl
              << "coil" << _detected << std::endl;
              */
        printf("anglefront: %d \n angleBack: %d \n", _dataFront, _dataBack);

    //        if (_detected){ // ne treba jer svakako proveramo da li je jednako 0xFF sto znaci da nema nista a iscitavanje regisra pocinje kada se
    //    se coil setuje

    if((_dataFront != 0xFF ) && (std::abs(previousState.angleFront - _dataFront) > ANGLE_DIFFERENCE)){
        previousState.detectionBrkonFront = true;
        previousState.angleFront = _dataFront;
        notification = new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,(_dataFront-50),true);
        printf("BRKON FRONT enemy ON: %d ", (_dataFront-50));
    }else if( _dataFront == 0xFF && previousState.detectionBrkonFront == true && !previousState.detectionSensorLeftFront && !previousState.detectionSensorRightFront){
        previousState.detectionBrkonFront = false;
        previousState.angleFront = 255;
        notification = new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,(previousState.angleFront - 50),false);
        debug("BRKON FRONT enemy OFF");
    }

    if((_dataBack != 0xFF) && (std::abs(previousState.angleBack - _dataBack) > ANGLE_DIFFERENCE)){
        previousState.detectionBrkonBack = true;
        previousState.angleBack = _dataBack;
        notification = new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK, (_dataBack + 135),true);
        printf("BRKON BACK enemy ON: %d ", (_dataBack+135));

    }else if( _dataBack == 0xFF && previousState.detectionBrkonBack == true && !previousState.detectionSensorBack){
        previousState.detectionBrkonBack = false;
        previousState.angleBack = 255;
        notification = new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK,(previousState.angleBack +135),false);
        debug("BRKON BACK eneme OFF");

        //        }
    }

    if (notification!=NULL){
        debug("Sending enemy detected notification!");
        sendNotification(notification);
    }

    /*
    BrkonCommand* command = new BrkonCommand(_dataFront, _dataBack, _detected);
    commandQueueLock.lock();
    //commandsToProcess.push(Instruction(command));
    commandQueueLock.unlock();
    queueNotEmpty.notify_one();
*/
}

void EnemyDetector::beaconDriverCallbackVeliki(short _veliki_cordX, short _veliki_cordY, bool _running){
    if(_running){
        // when beacon is running and getting valid data
        std::cout << "beacon VELIKI X: "
                  << _veliki_cordX << " Y: "
                  << _veliki_cordY << std::endl;
    }else{
        debug("beacon is not getting valide data");
    }

    if(_running){
        m_mutex.lock();
        enemyPosition.veliki_cordX = _veliki_cordX;
        enemyPosition.veliki_cordY = _veliki_cordY;
        enemyPosition.veliki_valid_data = true;
        m_mutex.unlock();
    }else{
        m_mutex.lock();
        enemyPosition.veliki_valid_data = false;
        m_mutex.unlock();

    };

    /*
    BeaconVelikiCommand* command = new BeaconVelikiCommand(_veliki_cordX,_veliki_cordY, _running);

    commandQueueLock.lock();
    commandsToProcess.push(Instruction(command));
    commandQueueLock.unlock();
   queueNotEmpty.notify_one();
   */
}


void EnemyDetector::beaconDriverCallbackMali(short _mali_cordX, short _mali_cordY, bool _running){

    if(_running){
        // when beacon is running and getting valid data
        std::cout << "beacon MALI X: "
                  << _mali_cordX << " Y: "
                  << _mali_cordY << std::endl;
    }else{
        debug("beacon is not getting valide data");
    }

    if(_running){
        beaconRetryCounter = 0;
        m_mutex.lock();
        enemyPosition.mali_cordX = _mali_cordX;
        enemyPosition.mali_cordY = _mali_cordY;
        enemyPosition.mali_valid_data = true;
        m_mutex.unlock();
    }else{
        m_mutex.lock();
        enemyPosition.mali_valid_data = false;
        m_mutex.unlock();

    }
    /*
    BeaconMaliCommand* command = new BeaconMaliCommand(_mali_cordX,_mali_cordY, _running);

    commandQueueLock.lock();
    commandsToProcess.push(Instruction(command));
    commandQueueLock.unlock();
    queueNotEmpty.notify_one();
*/
}

bool EnemyDetector::StopBeaconFunction(){
    beacon.stopBeacon();
    return true;
}

bool EnemyDetector::StartBeaconFunction(){
    beacon.startBeacon();
    return true;
}

bool EnemyDetector::BeaconMaliCallbackFunction(short _cordX, short _cordY, bool _running){
    if(_running){
        std::cout << "X: " << _cordX << " Y: " << _cordY << std::endl;
    }
    return true;
}

bool EnemyDetector::BeaconVelikiCallbackFunction(short _cordX, short _cordY, bool _running){
    if(_running){
        std::cout << "X: " << _cordX << " Y: " << _cordY << std::endl;
    }
    return true;
}

bool EnemyDetector::BrkonCallbackFunction(short _dataFront, short _dataBack, bool _detected){
    if(_detected){
        std::cout << "angle front: " << _dataFront << std::endl
                  << "angle back: " << _dataBack << std::endl;

    }else{

    }
    return true;
}

bool EnemyDetector::SensorCallbackFunction(int _id, bool _detected){

    debug("Wrong Command sensorCallbakFunction");

    return true;
}
/*
void EnemyDetector::main(){
    while(!shouldStop){
        EnemyPosition positionTmp;
        m_mutex.lock();
        positionTmp = enemyPosition;
        m_mutex.unlock();

        if( color == StartMessage::Color::YELLOW){
            // adjust positions to "normal ones"
            positionTmp.veliki_cordX -= 1500;
            positionTmp.veliki_cordY = 2000 - positionTmp.veliki_cordY;

            positionTmp.mali_cordX -= 1500;
            positionTmp.mali_cordY = 2000 - positionTmp.mali_cordY;
        }else{
            positionTmp.veliki_cordX = 1500 - positionTmp.veliki_cordX;
            // position Y is ok
            positionTmp.mali_cordX = 1500 - positionTmp.mali_cordX;
        }
        */
/*
        std::cout << "BEACON:" << std::endl
                  << "veliki  X: "  << positionTmp.veliki_cordX
                  << " Y: " << positionTmp.veliki_cordY
                  << std::endl
                  << "mali    X: " << positionTmp.mali_cordX
                  << " Y: " << positionTmp.mali_cordY
                  << std::endl;
        */
/*
        BeaconNotification* notification = new BeaconNotification(positionTmp.mali_valid_data,positionTmp.veliki_valid_data,
                                                                  positionTmp.mali_cordX, positionTmp.mali_cordY,
                                                                  positionTmp.veliki_cordX, positionTmp.veliki_cordY);
        //        debug("Sending enemy detected notification!");
        sendNotification(notification);

        boost::this_thread::sleep(boost::posix_time::milliseconds(2000));

    }
}
*/
/*
void EnemyDetector::stop(){
    shouldStop = true;
}
*/

bool EnemyDetector::StartDetectionFunction(){
#ifdef VELIKI_ROBOT
    frontLefttSensor.StartSensor();
    frontRightSensor.StartSensor();
    backSensor.StartSensor();
    brkon.startBrkon();
    //beacon.startBeacon();
#endif

#ifdef MALI_ROBOT
    frontLefttSensor.StartSensor();
    frontRightSensor.StartSensor();
    backSensor.StartSensor();

#endif
    return true;
}

bool EnemyDetector::StopDetectionFunction(){
#ifdef VELIKI_ROBOT
    frontLefttSensor.StopSensor();
    frontRightSensor.StopSensor();
    backSensor.StopSensor();
    brkon.stopBrkon();
    //beacon.stopBeacon();
#endif
#ifdef MALI_ROBOT

    frontLefttSensor.StopSensor();
    frontRightSensor.StopSensor();
    backSensor.StopSensor();

#endif

}

} // end namespace
