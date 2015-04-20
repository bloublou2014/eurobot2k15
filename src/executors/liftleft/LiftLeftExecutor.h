#ifndef _LIFTLEFTEXECUTOR_H
#define _LIFTLEFTEXECUTOR_H

#include "executors/AbstractLiftExecutor.h"

#include "utils/helper/LiftState.h"
#include "executors/msg/GetLiftState.h"
#include "executors/msg/LIftNotification.h"
#include "utils/modbus/ModbusClient.h"
#include "utils/modbus/ModbusSensoreClient.h"

using namespace modbus;
namespace executor {

class LiftLeftExecutor: public AbstractLiftExecutor, public ModbusSensorClientInterface2 {
public:
    LiftLeftExecutor():AbstractLiftExecutor(this->NAME), ModbusSensorClientInterface2(){    }

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

    ModbusSensorClient* modbusClient;
    void ProcessLiftLeftSensoreCallback();


};


}


#endif // _LIFTLEFTEXECUTOR_H
