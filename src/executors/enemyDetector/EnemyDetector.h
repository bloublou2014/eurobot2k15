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

    bool testBool;
    void testProcess();
    bool liftLoop();
};

} // end namespace
#endif // end ENEMY_DETECTOR
