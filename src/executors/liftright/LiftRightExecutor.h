#ifndef _LIFTRIGHTEXECUTOR_H
#define _LIFTRIGHTEXECUTOR_H

#include "executors/AbstractLiftExecutor.h"

#include "utils/helper/LiftState.h"
#include "executors/msg/GetLiftState.h"
#include "utils/modbus/ModbusSensoreClient.h"
#include "executors/msg/LIftNotification.h"

namespace executor {

class LiftRightExecutor: public AbstractLiftExecutor, public ModbusSensorClientInterface2{
public:
    LiftRightExecutor():AbstractLiftExecutor(this->NAME), ModbusSensorClientInterface2(){}
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
    ModbusSensorClient* modbusClient;
    bool liftProcess();

    void ProcessLiftRightSensoreCallback();


};


}


#endif // _LIFTRIGHTEXECUTOR_H
