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
using boost::condition_variable;
using boost::unique_lock;

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


    struct setSingleRegisterData{
        idData ID;
        short data;
    };

    enum InstructionType{
        SET_REGISTER,
        SET_COIL,
        STOP
    };


    struct Instruction{


        Instruction(){}
        Instruction(InstructionType _type, setSingleRegisterData _id_data): instruction(_type),id_data(_id_data){}

        InstructionType instruction;
        setSingleRegisterData id_data;
    };


    struct priorityValues{
        int priority;
        int counter;
    };


    ModbusClient();
    ModbusMaster* modbus;

    mutex* m_mutex;
    mutex queueLock;
    std::queue<setSingleRegisterData> registersToSet;
    std::queue<setSingleRegisterData> coilToSet;

    std::queue<Instruction> InstructionQueue;
    Instruction getNextInstruction();
    mutex InstructionQueueMutex;
    condition_variable queueNotEmpty;

    bool shouldStop = false;
    int delayTime = 5;
    int counter = 0;
    bool sensoreDelay = false;
    bool panic = false;

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

    bool writeToRegister(setSingleRegisterData data);
    bool writeToCoil(setSingleRegisterData data);

    bool setCoil(unsigned char _slave_address, short _function_address, short _data );
    bool setRegister(unsigned char _slave_address, short _function_address, short _data );

    void stopModbusClient();
    bool* getModbusSensorNotifier();
    bool* getModbusSensorPanic();

    bool getPanic();

};

}

#endif // _MODUS_CLIENT
