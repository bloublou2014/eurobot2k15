
#ifndef MODBUS_CLIENT_SWITZERLAND
#define MODBUS_CLIENT_SWITZERLAND

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

namespace modbus{


class ModbusClientSWInterface{
public:
    virtual void callbackCoilFunction(int _mapID, bool _detected){}
    virtual void callbackRegisterFunction(int _mapID, short _data){}
};

class ModbusClientSW : public Node {
private:
    enum statesEnum{
        CHECK = 0,
        WRITE = 2 ,
        READ = 3,
        DELAY = 1,
    };

    enum instructionType{
        WRITE_REGISTER,
        WRITE_COIL,
        STOP,
        SKIP,
        NONE,
    };

    enum regionType{
        REGISTER,
        COIL
    };

    enum errorType{
        ERROR,
        TRY_ONES,
        NO_ERROR
    };

    enum ReadSpeed{
        FAST,
        SLOW,
        NORMAL,
    };

    struct modbusID {
        unsigned char slave_address;
        short function_address;
    };

    struct writeData{
        modbusID id;
        short data;
    };


    struct Instruction{
        Instruction(){}
        Instruction(instructionType _instruction):instruction(STOP){}
        Instruction(instructionType _instruction, writeData _data): instruction(_instruction),data(_data){}

        instructionType instruction;
        writeData data;
    };

    struct callbackData{
        /*
        callbackData(ModbusClientSWInterface* _interface, modbusID _modID, regionType _region, bool* _reading,
                     bool* _detected, short _registerData): interface(_interface), modID(_modID), region(_region),
                     reading(_reading), detected(_detected), registerData(_registerData){}
        */
        ModbusClientSWInterface* interface; // address of class that has interented interface class
        modbusID modID; // slave i function address
        regionType region; // coil or regsiter
        bool reading; // should read this coil ol register
        bool detected; // if coil is on 1
        short registerData; // to store register readed data
        bool callbackOnNotDetected;
    };

    struct modbusConfig{
        int minimalDelay;
        int tryCounter;
        int delayCounterWrite;
        int delayCounterReadFast;
        int delayCounterReadSlow;
        int delayCounterReadNormal;
        int retryDelay;
        int errorTimeout;
    };


public:
    static ModbusClientSW* getModbusClientInstance();
    static ModbusClientSW* modbusClientSWInstance;

    bool setCoil(unsigned char _slave_address, short _function_address, short _data, bool _blocking = false ); // done
    bool setRegister(unsigned char _slave_address, short _function_address, short _data, bool _blocking = false ); // done

    bool readCoil(unsigned char _slave_address, short _function_address, bool* _callFunction); // done
    bool readRegister(unsigned char _slaveAddress, short _functionAddress, short* _data); // done

    int registerCoilReading(ModbusClientSWInterface* _interface,   // done
                             unsigned char _slave_address, short _function_address, bool _callbackOnNotDetected );
    int registerRegisterReading(ModbusClientSWInterface* _interface, // done
                                 unsigned char _slave_address, short _function_address );
    void setReading(int _id, bool _readingTrue);
    // TODO;

private:
    ModbusClientSW();
    bool writeRegister(writeData _data);
    bool writeCoil(writeData _data);
    Instruction getNextInstruction();

    Instruction currentInstruction;
    int counterWrite = 0;
    int counterRead = 0;
    int counterMap = 0;

    mutex* m_mutex;
    ModbusMaster* modbus;
    instructionType currentReadInstruction;
    instructionType currentWriteInstruction;
    statesEnum currentState = CHECK;
    statesEnum nextState = CHECK;
    errorType  errorElectronic = NO_ERROR;
    ReadSpeed readSpeed = FAST;
    bool shouldStop = false;
    modbusConfig config;

    typedef map <int, callbackData> callbackRegisterMapType;
    typedef map <int, callbackData>::iterator it_type;
    callbackRegisterMapType readingMap;
    int mapID = 0;

    std::queue<Instruction> setQueue;
    condition_variable setQueueNotEmpty;
    mutex InstructionQueueMutex;

    void main();
    void stop();
};


} // end namespace modbus
#endif // end MODBUS_CLIENT_SWITZERLAND
