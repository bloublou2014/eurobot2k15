#ifndef _EXECUTOR_COMMON
#define _EXECUTOR_COMMON

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <queue>
#include <map>

#include "AbstractExecutor.h"
#include "drivers/sensors/SensorDriver.h"
#include "drivers/actuators/Servos.h"
#include "executors/msg/ActuatorCommand.h"

#include "executors/ExecutorConfig.h"
#include "utils/modbus/ModbusSensoreClient.h"

using servo::ServoDriver;
using sensor::SensorDriver;
using namespace robot;
using boost::mutex;
using boost::condition_variable;
using boost::unique_lock;
using namespace modbus;

namespace executor {

class ExecutorCommon: public AbstractExecutor, public ExecutorConfig{
private:

public:
    ExecutorCommon(string _name):AbstractExecutor(_name), ExecutorConfig(), currentActuatorCommand(NULL), executorName(_name){}

protected:

    struct Instruction{
        enum Type{
            COMMAND,
            STOP
        };

        Instruction(Command* _command):command(_command),type(COMMAND){}
        Instruction(Type _type):type(_type),command(NULL){}

        Command* command;
        Type type;
    };


    void init();
    void stop();
    void main();

    typedef void( ExecutorCommon::*ActuatorCommandHandle)(ActuatorCommand* _actuatorCommand);
    typedef map<ActuatorType, ActuatorCommandHandle> ActuatorCommandHandleMap;
    ActuatorCommandHandleMap actuatorHandles;

    void processActuatorCommand(Command* _command);
    void processGetActuatorCommand(Command* _command);


    mutex commandQueueLock;
    queue<Instruction> commandsToProcess;

    Instruction getNextCommand();
    bool shouldStop;

    ActuatorCommand *currentActuatorCommand = NULL;

    string executorName;

    //bool ProcessSensorCallback();

    virtual void suscribe();
    virtual void mapping();
    virtual void suscribeToSensore(){}

    ConfigValues value;

    virtual bool UnloadPopcornFunction();
    virtual bool GetPopcornFunction();
    virtual bool KickRightFunction();
    virtual bool UnKickRightFunction();
    virtual bool KickLeftFunction();
    virtual bool UnKickLeftFunction();
    virtual bool GetObjectFunction();
    virtual bool UnloadObjectFunction();
    virtual bool SetSpeedFunction();
    virtual bool SetPositionFunction();
    virtual bool GetObjectStopFunction();
    virtual bool StartBrxonFunction();
    virtual bool StopBrxonFunction();
    virtual bool StartBeaconFunction();
    virtual bool StopBeaconFunction();
    virtual bool LeaveCarpetFunction();
    virtual bool CarpetLeaveFunction(){ return false; }
    virtual bool CarpetPosition1Function(){return false;}
    virtual bool CarpetPosition2Function(){return false; }
    virtual bool CarpetPositionOpenFunction(){ return false;}
    virtual bool CarpetPositionCloseFunction(){ return false;}
    virtual bool CallbackGetRightFunction(){ return false; }
    virtual bool CallbackGetLeftFunction(){ return false; }
    virtual bool StartDetectionFunction(){ return false; }
    virtual bool StopDetectionFunction(){ return false; }


protected:

    void reloadConfig(ActuatorCommand *_command);
    void getObject(ActuatorCommand* _command);
    void unloadObject(ActuatorCommand* _command);
    void getObjectStop(ActuatorCommand* _command);
    void kickRight(ActuatorCommand* _command);
    void unKickRight(ActuatorCommand* _command);
    void kickLeft(ActuatorCommand* _commmand);
    void unKickLeft(ActuatorCommand* _command);
    void unloadPopcorn(ActuatorCommand* _command);
    void getPopcorn(ActuatorCommand* _command);
    void callbackGetRight(ActuatorCommand* _commnad);
    void callbackGetLeft(ActuatorCommand* _commnad);

    void stopBrxon(ActuatorCommand* _command);
    void startBrxon(ActuatorCommand* _command);
    void stopBeacon(ActuatorCommand* _command);
    void startBeacon(ActuatorCommand* _command);

    void leaveCarpet(ActuatorCommand* _command);
    void carpetLeave(ActuatorCommand* _command);
    void carpetPosition1(ActuatorCommand* _command);
    void carpetPosition2(ActuatorCommand* _command);
    void carpetPositionOpen(ActuatorCommand* _command);
    void carpetPositionClose(ActuatorCommand* _command);

    void startDetection(ActuatorCommand* _command);
    void stopDetection(ActuatorCommand* _commnad);

    condition_variable queueNotEmpty;






};

}


#endif // _EXECUTOR_COMMON
