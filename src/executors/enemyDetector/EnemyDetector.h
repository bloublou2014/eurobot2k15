#ifndef ENEMY_DETECTOR
#define ENEMY_DETECTOR

#include "executors/ExecutorCommon.h"
#include "utils/modbus/ModbusSensoreClient.h"
#include "executors/msg/EnemyDetectedNotification.h"

using namespace modbus;

namespace executor {

class EnemyDetector: public ExecutorCommon, public ModbusSensorClientInterface2{
public:
    EnemyDetector():ExecutorCommon(this->NAME),ModbusSensorClientInterface2(){}
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
