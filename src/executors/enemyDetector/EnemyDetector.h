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
        bool detectionBack = false;
        bool detectionFront = false;
        bool detectionSensorBack = false;
        bool detectionSensorFront = false;
        bool detectionBrkonFront = false;
        bool detectionBrkonBack = false;
        int angleFront = 255;
        int angleBack = 255;
    };

    struct previousStateMali{
        bool sensorBack = false;
        bool sensorLeftFront = false;
        bool sensorRightFront = false;
    };

    struct EnemyPosition{
        short mali_cordX = 5000;
        short mali_cordY = 0;
        short veliki_cordX = 5000;
        short veliki_cordY = 0;
        bool mali_valid_data = false;
        bool veliki_valid_data = false;
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
    StartMessage::Color color = StartMessage::Color::YELLOW;
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
