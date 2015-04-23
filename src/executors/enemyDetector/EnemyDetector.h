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
    char previousFrontBrkon;
    char previousBackBrkon;

    static std::string NAME;
    void suscribe();
    ModbusSensorClient* modbusClient;
    void ProcessSensorCallback();
    void ProcessEnemySensorCallback1();
    void ProcessEnemySensorCallback2();
    void ProcessEnemySensorCallback3();
    void ProcessEnemySensorCallback4();
    void ProcessBeaconCallback();

    void ProcessNotEnemySensorCallback1();
    void ProcessNotEnemySensorCallback2();
    void ProcessNotEnemySensorCallback3();
    void ProcessNotEnemySensorCallback4();

    bool StartBeaconFunction();
    bool StopBeaconFunction();
    bool StartBrxonFunction();
    bool StopBrxonFunction();

    bool testBool = false;
    void testProcess();
};

} // end namespace
#endif // end ENEMY_DETECTOR
