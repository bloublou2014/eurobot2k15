#ifndef _LIFTLEFTEXECUTOR_H
#define _LIFTLEFTEXECUTOR_H

#include "executors/AbstractLiftExecutor.h"

#include "utils/helper/LiftState.h"
#include "executors/msg/GetLiftState.h"
#include "executors/msg/LIftNotification.h"
#include "utils/modbus/ModbusClient.h"
#include "utils/modbus/ModbusSensoreClient.h"
#include "utils/modbus/ModbusClientSwitzerland.h"
#include "drivers/sensors/SensorDriver.h"

using namespace modbus;
using namespace sensor;
namespace executor {

class LiftLeftExecutor: public AbstractLiftExecutor,/* public ModbusSensorClientInterface2 ,*/ public SensorDriverInterface  {
public:
    LiftLeftExecutor():AbstractLiftExecutor(this->NAME), /* ModbusSensorClientInterface2()*/ SensorDriverInterface() {}

    static string NAME;
    void suscribe();
    bool GetObjectFunction();
    bool UnloadObjectFunction();
    void mapping();
    void processGetLiftState(Command* _command);

    bool liftProcess();
private:
    LiftState lastState;

    boost::mutex stateLock;
    bool shouldCollect = false;
    bool CallbackGetLeftFunction();

    //ModbusSensorClient* modbusClient;
    //void ProcessLiftLeftSensoreCallback();

    // version SWI 1.0
    SensorDriver liftSensor;
    void SensorDriverCallback(int _id, bool _detected);
};


}


#endif // _LIFTLEFTEXECUTOR_H
