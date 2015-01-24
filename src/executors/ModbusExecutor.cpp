#include "ModbusExecutor.h"

#include <iostream>

using namespace std;

namespace modbus {
string ModbusExecutor::NAME="ModbusExecutor";


void ModbusExecutor::init(){
    this->registerCommand(ModbusCommand::NAME,static_cast<commandCallback>(&ModbusExecutor::processModbusCommand));
    //this->registerCommand(GetMotionState::NAME,static_cast<commandCallback>(&MotionExecutor::processGetMotionState));

    //motionHandles[MotionCommand::MotionType::MOVE_STRAIGHT]=static_cast<motionCommandHandle>(&MotionExecutor::moveForward);
    modbusHandles[ModbusCommand::ModbusType::MODBUS_FORCE_SINGLE_COIL]=static_cast<modbusCommandHandle>(&ModbusExecutor::modbusForceSingleCoil);
    modbusHandles[ModbusCommand::ModbusType::MODBUS_READ_COIL_STATUS]=static_cast<modbusCommandHandle>(&ModbusExecutor::modbusReadCoilStatus);
    modbusHandles[ModbusCommand::ModbusType::MODBUS_FORCE_MULTIPLE_COILS]=static_cast<modbusCommandHandle>(&ModbusExecutor::modbusForceMultipleCoils);
    modbusHandles[ModbusCommand::ModbusType::MODBUS_PRESET_SINGLE_REGISTER]=static_cast<modbusCommandHandle>(&ModbusExecutor::modbusPresetSingleRegister);
    modbusHandles[ModbusCommand::ModbusType::MODBUS_READ_HOLDING_REGISTERS]=static_cast<modbusCommandHandle>(&ModbusExecutor::modbusReadHoldingRegisters);

}

void ModbusExecutor::processModbusCommand(Command* command){
    commandQueueLock.lock();
    commandsToProcess.push(command);
    commandQueueLock.unlock();
}

//void ModbusExecutor::processGetMotionState(Command* command)

ModbusCommand* ModbusExecutor::getNextModbusCommand(){
    ModbusCommand* newCommand = NULL;
    commandQueueLock.lock();

    if (!commandsToProcess.empty()){
        while(commandsToProcess.size()>1){
            Command* cmd=commandsToProcess.front();
            commandsToProcess.pop();
            debug("Cant process command, have newer, sending error");
            sendResponseFromCommand(cmd,ERROR);
        }
        newCommand=(ModbusCommand*)commandsToProcess.front();
        commandsToProcess.pop();
    }
    commandQueueLock.unlock();
    return newCommand;
}

void ModbusExecutor::stop(){
    shouldStop= true;
}

void ModbusExecutor::main(){
    shouldStop=false;
       debug("Started main thread execution");

       // TODO
}
//1
void ModbusExecutor::modbusForceSingleCoil(ModbusCommand* _modbusCommand){
    ModbusForceSingleCoil* command=(ModbusForceSingleCoil*)_modbusCommand;
    debug("ModbusForceSingleCoil");
    currentModbusCommand=_modbusCommand;
    driver.ModbusForceSingleCoil(command->GetSlaveAddress(),command->GetAddress(),command->GetData());

}
//2
void ModbusExecutor::modbusReadCoilStatus(ModbusCommand* _modbusCommand){
    ModbusReadCoilStatus* command=(ModbusReadCoilStatus*)_modbusCommand;
    debug("ModbusReadCoilStatus");
    currentModbusCommand=_modbusCommand;
    driver.ModbusReadCoilStatus(command->GetSlaveAddress(), command->GetAddress(),command->GetQuantity(),command->GetData());

}
//3
void ModbusExecutor::modbusForceMultipleCoils(ModbusCommand* _modbusCommand){
    ModbusForceMultipleCoils* command=(ModbusForceMultipleCoils*)_modbusCommand;
    debug("ModbusForceMultipleCoils");
    currentModbusCommand=_modbusCommand;
    driver.ModbusForceMultipleCoils(command->GetSlaveAddress(), command->GetAddress(),command->GetQuantity(), command->GetData());
}
//4
void ModbusExecutor::modbusPresetSingleRegister(ModbusCommand* _modbusCommand){
    ModbusPresetSingleRegister* command=(ModbusPresetSingleRegister*)_modbusCommand;
    debug("ModbusPresetSingleRegister");
    currentModbusCommand=_modbusCommand;
    driver.ModbusPresetSingleRegister(command->GetSlaveAddress(), command->GetAddress(),command->GetData());
}
//5
void ModbusExecutor::modbusPresetMultipleRegs(ModbusCommand* _modbusCommand){
    ModbusPresetMultipleRegs* command=(ModbusPresetMultipleRegs*)_modbusCommand;
    debug("ModbusPresetMultipleRegs");
    currentModbusCommand=_modbusCommand;
    driver.ModbusPresetMultipleRegs(command->GetSlaveAddress(), command->GetAddress(),command->GetQuantity(),command->GetData());
}
//6
void ModbusExecutor::modbusReadHoldingRegisters(ModbusCommand* _modbusCommand){
    ModbusReadHoldingRegisters* command=(ModbusReadHoldingRegisters*)_modbusCommand;
    debug("ModbusReadHoldingRegisters");
    currentModbusCommand=_modbusCommand;
    driver.ModbusReadHoldingRegisters(command->GetSlaveAddress(), command->GetAddress(),command->GetQuantity(),command->GetData());
}


}
