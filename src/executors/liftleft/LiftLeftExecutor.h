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

class LiftLeftExecutor: public AbstractLiftExecutor, public ModbusSensorClientInterface {
public:
    LiftLeftExecutor():AbstractLiftExecutor(this->NAME),ModbusSensorClientInterface(){}
    ~LiftLeftExecutor(){modbusClient->stopModbusSensorClient();
                       modbus->stopModbusClient();}

    static string NAME;
    void suscribe();
    bool GetObjectFunction();
    bool UnloadObjectFunction();
    void mapping();
    void processGetLiftState(Command* _command);

    bool liftProcess();
    bool liftLoop();
private:
   LiftState lastState;
   boost::mutex stateLock;
   bool sensorDetect = false;
   void modbusEventReceived();
   void ProcessSensorCallback();
   ModbusClient* modbus;
   ModbusSensorClient* modbusClient;

};


}


#endif // _LIFTLEFTEXECUTOR_H
