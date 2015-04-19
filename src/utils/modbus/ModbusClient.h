#ifndef _MODBUS_CLIENT
#define _MODBUS_CLIENT

#include "ModbusMaster.h"
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "core/Node.h"
#include <map>
#include <queue>
#include <iterator>


using boost::mutex;
using boost::thread;
using namespace robot;
namespace modbus {

struct idData{
    unsigned char slaveAddress;
    short functionAddress;
};

class ModbusClientCallbackInterface{
public:
    bool readingSensore = false;

    ModbusClientCallbackInterface(){
        std::cout << "created interface" << std::endl;
    }

    virtual void modbusEventReceived();
};

class ModbusClient: public Node, public ModbusClientCallbackInterface{
public:

private:
    enum stateEnum{
        READ_SENSORE,
        READ_SERVO_STATE,        
        SET_REGISTER,
        MASTER_STATE,
        END,
        RESET_COUNTER
    };



    struct setSingleRegisterData{
        idData ID;
        short data;
    };

    struct priorityValues{
        int priority;
        int counter;
    };

    struct priorityType{
        priorityValues readSensore;
        priorityValues readServoState;
        priorityValues setRegister;
        priorityValues readRegister;
        int counter;
    };

    ModbusClient();
    ModbusMaster* modbus;

    mutex* m_mutex;
    mutex queueLock;
    std::queue<setSingleRegisterData> registersToSet;
    std::queue<setSingleRegisterData> coilToSet;

    priorityType priority;
    bool shouldStop = false;
    stateEnum state = RESET_COUNTER;
    int delayTime = 100;
    int counter = 0;
    bool sensoreDelay = false;
    bool panic = false;
    std::queue<stateEnum> stateQueue;

    void main();

    bool readCoilCallback();

    bool ModbusSensorClientNotifier;
    bool ModbusSensorClientPanic;


    friend bool operator< (const idData &id1, const idData &id2);
public:
    static ModbusClient* getMobusClientInstance();
    static ModbusClient* instance;

    bool readCoil(bool* _callFunction, idData _id);
    bool readRegister(short* _data, unsigned char _slaveAddress, short _functionAddress);

    bool writeToRegister();
    bool writeToCoil();

    bool setCoil(unsigned char _slave_address, short _function_address, short _data );
    bool setRegister(unsigned char _slave_address, short _function_address, short _data );

    void stopModbusClient();
    bool* getModbusSensorNotifier();
    bool* getModbusSensorPanic();

    bool getPanic();

};

}

#endif // _MODUS_CLIENT
