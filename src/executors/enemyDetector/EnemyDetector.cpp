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
    brkon.startBrkon();

    //dodati za beacon

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
    //debug("SENSOR CALLBACK ");
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
        debug("WROOONG ID ");
    }
    if (notification!=NULL){
        debug("Sending enemy detected notification!");
        sendNotification(notification);
    }
}

void EnemyDetector::brkonDriverCallback(short _data){
    std::cout <<" BRKON DATA : " << _data << std::endl;
}

} // end namespace
