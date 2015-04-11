#ifndef _ABSTRACTLIFTEXECUTOR_H
#define _ABSTRACTLIFTEXECUTOR_H

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <queue>
#include <map>

//#include "AbstractExecutor.h"
#include "ExecutorCommon.h"
#include "drivers/sensors/SensorDriver.h"
#include "drivers/actuators/Servos.h"
#include "executors/msg/ActuatorCommand.h"

#include "executors/ExecutorConfig.h"
#include"executors/ExecutorCommon.h"

/*
 * API
 * inherent abstract executor
 * constructor AbstractLiftExecutor( string_executor_name)
 * redefine virtual functions :
 *      suscribe ( add function for suscribe to messages )
 *      UnLoadFunction()
 *      GetObjectFunction()
 */

using servo::ServoDriver;
using sensor::SensorDriver;
using namespace robot;
using boost::mutex;

namespace executor {

class AbstractLiftExecutor: public ExecutorCommon {
private:


public:
    AbstractLiftExecutor(string _name):ExecutorCommon(_name){} //

    void SetDoorAddresses(unsigned char _slave_address, unsigned short _position_address, unsigned short _speed_address);
    void SetHandAddresses(unsigned char _slave_address, unsigned short _position_address, unsigned short _speed_address);
    void SetLiftAddresses(unsigned char _slave_address, unsigned short _position_address, unsigned short _speed_address);
    void SetSensorAddresses(unsigned char _slave_address, unsigned short _position_address);
    //void SetSensorFarAddresses(unsigned char _slave_address, unsigned short _position_address);

protected:
    enum HandFunctioType{ OPEN,
                          CLOSE};

    enum DoorFunctionType { OPEN_GET,
                            OPEN_LEAVE,
                            CLOSE_
                          };

    enum LiftFunctiontype{ LEVEL0,
                           LEVEL1,
                           LEVEL2};


    virtual void suscribe();
    virtual void maping();

    virtual bool GetObjectFunction();
    virtual bool UnloadObjectFunction();
    virtual bool SetSpeedFunction();
    virtual bool SetPositionFunction();

    //LiftExecutorConfig value;
    //ConfigValues value;
    string executorName;
    ServoDriver door;
    ServoDriver hand;
    ServoDriver lift;
    SensorDriver sensor;

    virtual bool doorF(DoorFunctionType _option);
    virtual bool handF(HandFunctioType _option);
    virtual bool liftF(LiftFunctiontype _option);
    virtual void delayF();
    virtual void delayF(int _millisecondes);
    virtual bool sensorF();
    virtual bool doorS();
    virtual bool handS();
    virtual bool liftS();

    virtual void brodcastNotification();

private:

    void getObject(ActuatorCommand* _actionCommand);
    void unloadObject(ActuatorCommand* _actionCommand);
    //void kick(ActuatorCommand * _command);
    //void getPopcorn(ActuatorCommand* _command);
    //void unloadPopcorn(ActuatorCommand* _command);
    //void setSpeed(ActuatorCommand* _command);
    //void setPosition(ActuatorCommand* _command);
    //void reloadConfig(ActuatorCommand* _command);

    LiftFunctiontype currentLevel = LEVEL0;

    
};

}



#endif // _ABSTRACTLIFTEXECUTOR_H
