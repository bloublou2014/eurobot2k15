#ifndef ENEMY_DETECTOR
#define ENEMY_DETECTOR

#include "executors/ExecutorCommon.h"
#include "utils/modbus/ModbusSensoreClient.h"

using namespace modbus;

namespace executor {
class EnemyDetector: public ExecutorCommon, public ModbusSensorClientInterface{
public:
    EnemyDetector():ExecutorCommon(this->NAME),ModbusSensorClientInterface(){}

private:
    static std::string NAME;
    void suscribe();
    ModbusSensorClient* modbusClient;
    void ProcessSensorCallback();
    void ProcessEnemySensorCallback1();
    void ProcessEnemySensorCallback2();
    void ProcessEnemySensorCallback3();
    void ProcessEnemySensorCallback4();
    void ProcessBeaconCallback();

    bool testBool = false;
    void testProcess();
    bool liftLoop();
};

} // end namespace
#endif // end ENEMY_DETECTOR
