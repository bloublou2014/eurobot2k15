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

using servo::ServoDriver;
using sensor::SensorDriver;
using namespace robot;
using boost::mutex;

namespace executor {

class ExecutorCommon: public AbstractExecutor, public ExecutorConfig{
private:

public:
    ExecutorCommon(string _name):AbstractExecutor(_name), ExecutorConfig(), currentActuatorCommand(NULL), executorName(_name){}

    void init();
    void stop();
    void main();

    typedef void( ExecutorCommon::*ActuatorCommandHandle)(ActuatorCommand* _actuatorCommand);
    typedef map<ActuatorType, ActuatorCommandHandle> ActuatorCommandHandleMap;
    ActuatorCommandHandleMap actuatorHandles;

    void processActuatorCommand(Command* _command);
    void processGetActuatorCommand(Command* _command);

    mutex commandQueueLock;
    queue<Command*> commandsToProcess;

    ActuatorCommand* getNextCommand();
    bool shouldStop;

    ActuatorCommand *currentActuatorCommand = NULL;

    string executorName;

protected:

    virtual void suscribe();
    virtual void mapping();
    virtual void brodcastNotification();

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
    virtual bool liftLoop();

private:

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






};

}


#endif // _EXECUTOR_COMMON
