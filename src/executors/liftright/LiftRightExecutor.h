#ifndef _LIFTRIGHTEXECUTOR_H
#define _LIFTRIGHTEXECUTOR_H

#include "executors/AbstractLiftExecutor.h"

#include "utils/helper/LiftState.h"
#include "executors/msg/GetLiftState.h"
#include "utils/modbus/ModbusSensoreClient.h"
#include "executors/msg/LIftNotification.h"

namespace executor {

class LiftRightExecutor: public AbstractLiftExecutor, public ModbusSensorClientInterface {
public:
    LiftRightExecutor():AbstractLiftExecutor(this->NAME), ModbusSensorClientInterface(){}
    static string NAME;
    void suscribe();
    bool GetObjectFunction();
    bool UnloadObjectFunction();
    void brodcastNotification();
    void mapping();
private:
    void processGetLiftState(Command* _command);
    LiftState lastState;
    boost::mutex stateLock;
    bool shouldCollect = false;
    void ProcessSensorCallback();
    bool liftLoop();
    bool liftProcess();
    ModbusSensorClient* modbusClient;

};


}


#endif // _LIFTRIGHTEXECUTOR_H
