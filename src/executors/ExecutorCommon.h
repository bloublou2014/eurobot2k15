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
#include "executors/msg/EnemyDetectorCommand.h"

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
    virtual bool CarpetLeaveFunction(){ std::cout << "REDEFINE " << std::endl;  return false; }
    virtual bool CarpetPosition1Function(){std::cout << "REDEFINE " << std::endl;return false;}
    virtual bool CarpetPosition2Function(){std::cout << "REDEFINE " << std::endl;return false; }
    virtual bool CarpetPositionOpenFunction(){ std::cout << "REDEFINE " << std::endl;return false;}
    virtual bool CarpetPositionCloseFunction(){ std::cout << "REDEFINE " << std::endl;return false;}
    virtual bool CallbackGetRightFunction(){std::cout << "REDEFINE " << std::endl; return false; }
    virtual bool CallbackGetLeftFunction(){std::cout << "REDEFINE " << std::endl; return false; }
    virtual bool StartDetectionFunction(){ std::cout << "REDEFINE " << std::endl;return true; }
    virtual bool StopDetectionFunction(){ std::cout << "REDEFINE " << std::endl;return true; }

    virtual bool BeaconMaliCallbackFunction(short _cordX, short _cordY, bool _running){ std::cout << "REDEFINE " << std::endl;return false;}
    virtual bool BeaconVelikiCallbackFunction(short _cordX, short _cordY, bool _running){ std::cout << "REDEFINE " << std::endl;return false;}
    virtual bool BrkonCallbackFunction(short _dataFront, short _dataBack, bool _detected){ std::cout << "REDEFINE " << std::endl;return false;}
    virtual bool SensorCallbackFunction(int _id, bool _detected){ std::cout << "REDEFINE " << std::endl;return false;}

    //added for new srednji mehanizam

    virtual bool GetGlassFunction(){ return false; }
    virtual bool LeaveGlassFunction(){ return false; }
    virtual bool GetBallFunction(){ return false; }
    virtual bool LeaveBallFunction(){ return false; }



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

    // enemy detector Command
    void sensorCommand(ActuatorCommand* _command);
    void beaconMaliCommand(ActuatorCommand* _command);
    void beaconVelikiCommand(ActuatorCommand* _command);
    void brkonCommand(ActuatorCommand* _command);

    // added for new "srednji mehanizam


    condition_variable queueNotEmpty;






};

}


#endif // _EXECUTOR_COMMON
