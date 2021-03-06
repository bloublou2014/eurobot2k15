#ifndef _LIFTRIGHTEXECUTOR_H
#define _LIFTRIGHTEXECUTOR_H

#include "executors/AbstractLiftExecutor.h"

#include "utils/helper/LiftState.h"
#include "executors/msg/GetLiftState.h"
#include "utils/modbus/ModbusSensoreClient.h"
#include "executors/msg/LIftNotification.h"
#include "drivers/sensors/SensorDriver.h"

using namespace sensor;
namespace executor {

class LiftRightExecutor: public AbstractLiftExecutor, /*public ModbusSensorClientInterface2*/ public SensorDriverInterface  {
public:
    LiftRightExecutor():AbstractLiftExecutor(this->NAME), /*ModbusSensorClientInterface2()*/ SensorDriverInterface(){}
    static string NAME;
    void suscribe();
    bool GetObjectFunction();
    bool UnloadObjectFunction();
    void mapping();
private:
    void processGetLiftState(Command* _command);
    LiftState lastState;
    boost::mutex stateLock;
    bool shouldCollect = false;
    bool CallbackGetRightFunction();
    //ModbusSensorClient* modbusClient;
    bool liftProcess();

    //void ProcessLiftRightSensoreCallback();
    SensorDriver liftSensor;
    void SensorDriverCallback(int _id, bool _detected);

};


}


#endif // _LIFTRIGHTEXECUTOR_H
