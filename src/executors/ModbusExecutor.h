#ifndef _MODBUSEXECUTOR_H
#define _MODBUSEXECUTOR_H

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <queue>
#include <map>

#include "AbstractExecutor.h"
#include "executors/msg/ModbusCommand.h"
#include "executors/msg/ModbusNotification.h"
#include "utils/modbus/ModbusMaster.h"

//#include "executors/msg/MotionCommand.h"
//#include "executors/msg/GetMotionState.h"
//#include "drivers/motion/MotionDriver.h"
//#include "executors/msg/MotionNotification.h"


using namespace robot;
using boost::mutex;
using std::queue;

namespace modbus{

class ModbusExecutor: public AbstractExecutor{
public:
    static string NAME;
    ModbusExecutor():AbstractExecutor(NAME),currentModbusCommand(NULL){}

    void init();
    void stop();

    void processModbusCommand(Command* command);
    void processGetModbusState(Command* command);
protected:
    void main();
private:
    mutex commandQueueLock;
    queue<Command*> commandsToProcess;
    ModbusCommand* getNextModbusCommand();

    bool shouldStop;
    mutex stateLock;
    //ModbusState lastState;

    ModbusCommand* currentModbusCommand;
    ModbusMaster driver;
    typedef void (ModbusExecutor::*modbusCommandHandle)(ModbusCommand* _modbusCommand);
    typedef map<ModbusCommand::ModbusType,modbusCommandHandle> ModbusCommandsHandleMap;
    ModbusCommandsHandleMap modbusHandles;
    /*
    void moveToPosition(MotionCommand* _motionCommand);
    void moveForward(MotionCommand* _motionCommand);
    void rotateFor(MotionCommand* _motionCommand);
    void rotateTo(MotionCommand* _motionCommand);
    void moveArc(MotionCommand* _motionCommand);
    void stopMovement(MotionCommand* _motionCommand);
    */

    void modbusPresetSingleRegister(ModbusCommand* _modbusCommand);
    void modbusForceSingleCoil(ModbusCommand* _modbusCommand);
    void modbusPresetMultipleRegs(ModbusCommand* _modbusCommand);
    void modbusForceMultipleCoils(ModbusCommand* _modbusCommand);
    void modbusReadCoilStatus(ModbusCommand* _modbusCommand);
    void modbusReadHoldingRegisters(ModbusCommand* _modbusCommand);

};

}

#endif //_MODBUSEXECUTOR_H
