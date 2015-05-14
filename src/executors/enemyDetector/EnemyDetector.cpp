#include "EnemyDetector.h"

namespace executor{

std::string EnemyDetector::NAME = "EnemyDetectorExecutor";


void EnemyDetector::suscribe(){

    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&EnemyDetector::processActuatorCommand));

#ifdef VELIKI_ROBOT
    frontSensor.setConfig(char(4),char(1),sensorFrontID,this, true);
    frontSensor.RegisterSensor();
    frontSensor.StartSensor();

    backSensor.setConfig(char(4), char(5),sensorBackID, this, true);
    backSensor.RegisterSensor();
    backSensor.StartSensor();

    brkon.setCoilConfig(char(4),char(7));
    brkon.setRegisterConfig(char(4),char(1));
    brkon.setPowerCoilConfig(char(4),char(8));
    brkon.registerInerface(this);
    brkon.registerBrkon();
//    brkon.startBrkon();

    //TODO
    //dodati za beacon
    beacon.setBeaconConfig(char(2),char(1),char(2),char(2),char(3),char(4),char(2),char(1),char(2),2,this);
//    beacon.registerBeacon();
    //    beacon.startBeacon();


#endif

#ifdef MALI_ROBOT

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
        if(previousState.sensorBack != _detected){
            previousState.sensorBack = _detected;

            if(_detected){
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK,180,true);
                debug("DOSO BACK");
            }else{
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK,180,false);
                debug("OTISAO BACK");
            }
        }
        backSensor.StartSensor();
    }else if(_id ==this->sensorFrontID || (_id == frontLeftSensorID) || ( _id == frontRightSensorID )){
        if(previousState.sensorFront != _detected){
            previousState.sensorFront = _detected;

            if(_detected){
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,0,true);
                debug("DOSO FRONT");
            }else{
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,0,false);
                debug("OTISAO FRONT");
            }
        }
        frontSensor.StartSensor();
    }else{
        debug("WROOONG ID ");
    }

}

void EnemyDetector::brkonDriverCallback(unsigned char _dataFront, unsigned char _dataBack, bool _detected){
    EnemyDetectedNotification* notification=NULL;
/*
    std::cout << "angle front: " << _dataFront << std::endl
              << "angle back: " << _dataBack << std::endl
              << "coil" << _detected << std::endl;
              */
    //printf("anglefront: %d \n angleBack: %d \n", _dataFront, _dataBack);

    if (_detected){
        if((_dataFront != 0xFF ) && (std::abs(previousState.angleFront - _dataFront) > ANGLE_DIFFERENCE)){
            previousState.angleFrontDetected = true;
            previousState.angleFront = _dataFront;
            notification = new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,(_dataFront-50),true);
            printf("BRKON FRONT enemy on: %d ", (_dataFront-50));
        }else if( _dataFront == 0xFF && previousState.angleFrontDetected == true){
            previousState.angleFrontDetected = false;
            notification = new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,(previousState.angleFront - 50),false);
            debug("BRKON FRONT enemy OFF");
        }

        if((_dataBack != 0xFF) && (std::abs(previousState.angleBack - _dataBack) > ANGLE_DIFFERENCE)){
            previousState.angleBackDetected = true;
            previousState.angleBack = _dataBack;
            notification = new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK, (_dataBack + 135),true);
            printf("BRKON BACK enemy on: %d ", (_dataBack+135));

        }else if( _dataBack == 0xFF && previousState.angleBackDetected == true){
            previousState.angleBackDetected = false;
            notification = new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK,(previousState.angleBack +135),false);
            debug("BRKON BACK eneme OFF");

        }
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
    EnemyDetectedNotification* notification=NULL;
    if(_id == this->sensorBackID){
        if(previousState.sensorBack != _detected){
            previousState.sensorBack = _detected;

            if(_detected){
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK,0,true);
                debug("DOSO BACK");
            }else{
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK,0,false);
                debug("OTISAO BACK");
            }
        }
        backSensor.StartSensor();
    }else if(_id ==this->sensorFrontID){
        if(previousState.sensorFront != _detected){
            previousState.sensorFront = _detected;

            if(_detected){
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,0,true);
                debug("DOSO FRONT");
            }else{
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,0,false);
                debug("OTISAO FRONT");
            }
        }
        frontSensor.StartSensor();
    }else{
        debug("WROOONG ID MSSG ");
    }
    if (notification!=NULL){
        debug("Sending enemy detected notification!");
        sendNotification(notification);
    }
    return true;
}

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

    std::cout << "BEACON:" << std::endl
              << "veliki  X: "  << positionTmp.veliki_cordX
              << " Y: " << positionTmp.veliki_cordY
              << std::endl
              << "mali    X: " << positionTmp.mali_cordX
              << " Y: " << positionTmp.mali_cordY
              << std::endl;
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

    }
}

void EnemyDetector::stop(){
    shouldStop = true;
}

} // end namespace
