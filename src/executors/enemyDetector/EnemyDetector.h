#ifndef ENEMY_DETECTOR
#define ENEMY_DETECTOR

#include "executors/ExecutorCommon.h"
//#include "utils/modbus/ModbusSensoreClient.h"
#include "executors/msg/EnemyDetectedNotification.h"
#include "executors/msg/EnemyDetectorCommand.h"
#include "executors/msg/ActuatorCommand.h"

#include "boost/thread/mutex.hpp"
#include "messages/StartMatchMessage.h"

#include "drivers/sensors/BeaconDriver.h"
#include "drivers/sensors/BrkonDriver.h"
#include "drivers/sensors/SensorDriver.h"

using namespace modbus;
using namespace sensor;
using namespace robot;

namespace executor {

class EnemyDetector: public ExecutorCommon, public BeaconDriverInterface, public BrkonDriverInterface, public SensorDriverInterface{
private:
    struct previousStateVeliki{
        bool sensorBack = false;
        bool sensorFront = false;
        bool angleFrontDetected = false;
        bool angleBackDetected = false;
        int angleFront = 0;
        int angleBack = 0;
    };

    struct previousStateMali{
        bool sensorBack = false;
        bool sensorLeftFront = false;
        bool sensorRightFront = false;
    };

    struct EnemyPosition{
        short mali_cordX;
        short mali_cordY;
        short veliki_cordX;
        short veliki_cordY;
        bool mali_valid_data;
        bool veliki_valid_data;
    };




public:
    EnemyDetector():ExecutorCommon(this->NAME),BeaconDriverInterface(),BrkonDriverInterface(),SensorDriverInterface(){}
    ~EnemyDetector(){ brkon.stopBrkon();}
private:
    static string NAME;
    void suscribe();
    void main();
    void stop();
    // callbacs for sensors
    void brkonDriverCallback(unsigned char _dataFront, unsigned char _dataBack, bool _detected);
    void beaconDriverCallbackVeliki(short _veliki_cordX, short _veliki_cordY, bool _running);
    void beaconDriverCallbackMali(short _mali_cordX, short _mali_cordY, bool _running);
    void SensorDriverCallback(int _id, bool _detected);

    bool StartBrxonFunction();
    bool StopBrxonFunction();
    bool StartBeaconFunction();
    bool StopBeaconFunction();
    bool BeaconMaliCallbackFunction(short _cordX, short _cordY, bool _running);
    bool BeaconVelikiCallbackFunction(short _cordX, short _cordY, bool _running);
    bool BrkonCallbackFunction(short _dataFront, short _dataBack, bool _detected);
    bool SensorCallbackFunction(int _id, bool _detected);

    boost::mutex m_mutex;
    StartMessage::Color color;
    int sensorFrontID = 1;
    int sensorBackID = 2;

    int backSesnorID = 3;
    int frontLeftSensorID = 4;
    int frontRightSensorID = 5;

#ifdef VELIKI_ROBOT

    SensorDriver backSensor;
    SensorDriver frontSensor;
    BeaconDriver beacon;
    BrkonDriver brkon;

    previousStateVeliki previousState;
    EnemyPosition enemyPosition;

    int beaconRetryCounter = 0;
    const int BEACON_RETRAY = 5;
    const int ANGLE_DIFFERENCE = 20;

#endif

#ifdef MALI ROBOT
    SensorDriver backSensor;
    SensorDriver frontLefttSensor;
    SensorDriver frontRightSensor;


#endif



};

} // end namespace
#endif // end ENEMY_DETECTOR
