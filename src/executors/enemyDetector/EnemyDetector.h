#ifndef ENEMY_DETECTOR
#define ENEMY_DETECTOR

#include "executors/ExecutorCommon.h"
#include "utils/modbus/ModbusSensoreClient.h"
#include "executors/msg/EnemyDetectedNotification.h"

using namespace modbus;

namespace executor {

class EnemyDetector: public ExecutorCommon, public ModbusSensorClientInterface2{
public:
<<<<<<< HEAD
    EnemyDetector():ExecutorCommon(this->NAME),ModbusSensorClientInterface2(),
        detectedAngle(1800),enemyDetected(false){}
=======
    EnemyDetector():ExecutorCommon(this->NAME),ModbusSensorClientInterface(){}
>>>>>>> c4ce73a9cf805406b9f4079ba5f036e4740d82ee
    ~EnemyDetector(){ modbusClient->stopBecaon();
                      modbusClient->stopBecaon();
                    }
private:
    enemyDetectedStruct previousState;

    static std::string NAME;
    void suscribe();
    ModbusSensorClient* modbusClient;
    void ProcessSensorCallback();
    void ProcessEnemySensorCallback1();
    void ProcessEnemySensorCallback2();
    void ProcessEnemySensorCallback3();
    void ProcessEnemySensorCallback4();
    void ProcessBeaconCallback();

    bool StartBeaconFunction();
    bool StopBeaconFunction();
    bool StartBrxonFunction();
    bool StopBrxonFunction();

    bool testBool = false;
    void testProcess();
    bool liftLoop();
};

} // end namespace
#endif // end ENEMY_DETECTOR
