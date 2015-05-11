#ifndef ENEMY_DETECTOR
#define ENEMY_DETECTOR

#include "executors/ExecutorCommon.h"
//#include "utils/modbus/ModbusSensoreClient.h"
#include "executors/msg/EnemyDetectedNotification.h"
#include "executors/msg/EnemyDetectorCommand.h"

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
        int angle = 0;
    };

public:
    EnemyDetector():ExecutorCommon(this->NAME),BeaconDriverInterface(),BrkonDriverInterface(),SensorDriverInterface(){}
    ~EnemyDetector(){ }
private:
    static string NAME;
    void suscribe();
    // callbacs for sensors
    void brkonDriverCallback(short _data);
    void beaconDriverCallbackVeliki(short _veliki_cordX, short _veliki_cordY, bool _running){}
    void beaconDriverCallbackMali(short _mali_cordX, short _mali_cordY, bool _running){}
    void SensorDriverCallback(int _id, bool _detected);

    bool StartBrxonFunction();
    bool StopBrxonFunction();

#ifdef VELIKI_ROBOT

    SensorDriver backSensor;
    SensorDriver frontSensor;
    BeaconDriver beacon;
    BrkonDriver brkon;
    int sensorFrontID = 1;
    int sensorBackID = 2;

    previousStateVeliki previousState;

#endif

#ifdef MALI ROBOT
    SensorDriver backSensor;
    SensorDriver frontLefttSensor;
    SensorDriver frontRightSensor;
#endif



};

} // end namespace
#endif // end ENEMY_DETECTOR
