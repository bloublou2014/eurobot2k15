#ifndef _MODBUSCOMMAND_H
#define _MODBUSCOMMNAD_H

#include "messages/Command.h"
#include "messages/CommandResponse.h"

#include "utils/modbus/ModbusMaster.h"

using namespace robot;

namespace modbus {

class ModbusCommand: public Command {
public:
    static string NAME;
    enum ModbusType{
        MODBUS_FORCE_SINGLE_COIL,
        MODBUS_READ_COIL_STATUS,
        MODBUS_FORCE_MULTIPLE_COILS,
        MODBUS_PRESET_SINGLE_REGISTER,
        MODBUS_PRESET_MULTIPLE_REGS,
        MODBUS_READ_HOLDING_REGISTERS};

    ModbusCommand(ModbusType _type):Command("ModbusCommand", "MotionExecutor"), type(_type){}
    ModbusType getModbusType() const { return type; }
private:
    ModbusType type;
};

class ModbusForceSingleCoil: public ModbusCommand{
public:
    ModbusForceSingleCoil(unsigned char _slave_address, unsigned short _address, signed char _data):ModbusCommand(MODBUS_FORCE_SINGLE_COIL), slave_address(_slave_address), address(_address), data(_data){}

    unsigned char GetSlaveAddress() const {return slave_address;}
    unsigned short GetAddress() const {return address;}
    signed char GetData() const {return data;}
private:
    unsigned char slave_address;
    unsigned short address;
    signed char data;
};

class ModbusReadCoilStatus: public ModbusCommand{
public:
    ModbusReadCoilStatus(unsigned char _slave_address, unsigned short _address, unsigned short _quantity, signed char *_data):ModbusCommand(MODBUS_READ_COIL_STATUS), slave_address(_slave_address), address(_address), quantity(_quantity), data(_data){}

    unsigned char GetSlaveAddress() const {return slave_address;}
    unsigned short GetAddress() const {return address;}
    unsigned short GetQuantity() const {return quantity;}
    signed char* GetData() const {return data;}
private:
    unsigned char slave_address;
    unsigned short address;
    unsigned short quantity;
    signed char* data;

};

class ModbusForceMultipleCoils: public ModbusCommand{
public:
    ModbusForceMultipleCoils(unsigned char _slave_address, unsigned short _address, unsigned short _quantity, unsigned char *_data):ModbusCommand(MODBUS_FORCE_MULTIPLE_COILS), slave_address(_slave_address), address(_address), quantity(_quantity), data(_data){}

    unsigned char GetSlaveAddress() const {return slave_address;}
    unsigned short GetAddress() const {return address;}
    unsigned short GetQuantity() const {return quantity;}
    unsigned char* GetData() const {return data;}
private:
    unsigned char slave_address;
    unsigned short address;
    unsigned short quantity;
    unsigned char* data;
};
class ModbusPresetSingleRegister: public ModbusCommand{
public:
    ModbusPresetSingleRegister(unsigned char _slave_address, unsigned short _address, short _data):ModbusCommand(MODBUS_PRESET_SINGLE_REGISTER),slave_address(_slave_address), address(_address), data(_data){}

    unsigned char GetSlaveAddress() const {return slave_address;}
    unsigned short GetAddress() const {return address;}
    signed char GetData() const {return data;}
private:
    unsigned char slave_address;
    unsigned short address;
    signed char data;
};

class ModbusPresetMultipleRegs: public ModbusCommand{
public:
    ModbusPresetMultipleRegs(unsigned char _slave_address, unsigned short _address, unsigned short _quantity, short *_data):ModbusCommand(MODBUS_PRESET_MULTIPLE_REGS), slave_address(_slave_address), address(_address), quantity(_quantity), data(_data){}

    unsigned char GetSlaveAddress() const {return slave_address;}
    unsigned short GetAddress() const {return address;}
    unsigned short GetQuantity() const {return quantity;}
    short* GetData() const {return data;}
private:
    unsigned char slave_address;
    unsigned short address;
    unsigned short quantity;
    short* data;
};

class ModbusReadHoldingRegisters: public ModbusCommand{
public:
    ModbusReadHoldingRegisters(unsigned char _slave_address, unsigned short _address, unsigned short _quantity, short *_data):ModbusCommand(MODBUS_READ_HOLDING_REGISTERS), slave_address(_slave_address), address(_address), quantity(_quantity), data(_data){}

    unsigned char GetSlaveAddress() const {return slave_address;}
    unsigned short GetAddress() const {return address;}
    unsigned short GetQuantity() const {return quantity;}
    short* GetData() const {return data;}
private:
    unsigned char slave_address;
    unsigned short address;
    unsigned short quantity;
    short* data;

};

}

#endif
