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

    //typedef map <idData, ModbusClientCallbackInterface*> callbackRegisterMapType;
   //typedef map <idData, ModbusClientCallbackInterface*>::iterator it_type;
    //callbackRegisterMapType callbackRegisterMap;
    //it_type it_check ;

    mutex* m_mutex;
    mutex queueLock;
    std::queue<setSingleRegisterData> registersToSet;

    priorityType priority;
    bool shouldStop = false;
    stateEnum state = RESET_COUNTER;
    int delayTime = 5;
    int counter = 0;
    bool sensoreDelay = false;
    std::queue<stateEnum> stateQueue;

    //void calculatePriority(priorityType* _priority);
    void main();

    bool readCoilCallback();


    friend bool operator< (const idData &id1, const idData &id2);
public:
    static ModbusClient* getMobusClientInstance();
    static ModbusClient* instance;

    bool readCoil(bool* _callFunction, idData _id);
    //void registerToModbusCoilCallback(unsigned char _slave_address,unsigned short _coil_address, bool on_bool_callback, ModbusClientCallbackInterface* obj);
    bool writeToRegister();
    bool setRegister(unsigned char _slave_address, short _function_address, short _data );
    void stopModbusClient();

};

}

#endif // _MODUS_CLIENT
