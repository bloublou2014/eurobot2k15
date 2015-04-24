#include "ExecutorCommon.h"

namespace executor{

void ExecutorCommon::init(){

    actuatorHandles[ActuatorType::RELOAD_CONFIG]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::reloadConfig);
    actuatorHandles[ActuatorType::GET_POPCORN]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::getPopcorn);
    actuatorHandles[ActuatorType::UNLOAD_POPCORN]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::unloadPopcorn);
    actuatorHandles[ActuatorType::KICK_RIGHT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::kickRight);
    actuatorHandles[ActuatorType::KICK_LEFT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::kickLeft);
    actuatorHandles[ActuatorType::UNKICK_RIGHT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::unKickRight);
    actuatorHandles[ActuatorType::UNKICK_LEFT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::unKickLeft);
    actuatorHandles[ActuatorType::GET_OBJECT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::getObject);
    actuatorHandles[ActuatorType::UNLOAD_OBJECT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::unloadObject);
    actuatorHandles[ActuatorType::RELOAD_CONFIG]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::reloadConfig);
    actuatorHandles[ActuatorType::GET_OBJECT_STOP]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::getObjectStop);
    actuatorHandles[ActuatorType::START_BRXON]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::startBrxon);
    actuatorHandles[ActuatorType::STOP_BRXON]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::stopBrxon);
    actuatorHandles[ActuatorType::START_BEACON]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::startBeacon);
    actuatorHandles[ActuatorType::STOP_BEACON]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::stopBeacon);
    actuatorHandles[ActuatorType::LEAVE_CARPET]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::leaveCarpet);
    actuatorHandles[ActuatorType::CALLBACK_GET_LEFT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::callbackGetLeft);
    actuatorHandles[ActuatorType::CALLBACK_GET_RIGHT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::callbackGetRight);
    actuatorHandles[ActuatorType::START_DETECTION]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::startDetection);
    actuatorHandles[ActuatorType::STOP_DETECTION]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::stopDetection);



    suscribeToSensore();
    suscribe();
    mapping();

}




void ExecutorCommon::processActuatorCommand(Command *_command){
    commandQueueLock.lock();
    commandsToProcess.push(Instruction(_command));
    commandQueueLock.unlock();
    queueNotEmpty.notify_one();
}

ExecutorCommon::Instruction ExecutorCommon::getNextCommand(){  // if there is more then one command = send error to old one onda execute new one , returns new command
    unique_lock<boost::mutex> lock(commandQueueLock);
    ActuatorCommand* newCommand = NULL;

    //while (instructionQueue.empty()) {
    //    queueNotEmpty.wait(lock);
    //}

    while (commandsToProcess.empty()) {
        queueNotEmpty.wait(lock);
    }

        while(commandsToProcess.size() > 1){
            Instruction cmd = commandsToProcess.front();
            commandsToProcess.pop();
            debug("newer Command, seding error to old");
            ActuatorCommand* cmdAct = cmd.command;

            sendResponseFromCommand(cmdAct, ERROR);
        }
        Instruction newInst = commandsToProcess.front();
        commandsToProcess.pop();
        //newCommand = newInst.command;

    return newInst;
}

void ExecutorCommon::main(){
    shouldStop = false;
    while(!shouldStop){

        Instruction inst = getNextCommand();

        if(inst.type == Instruction::STOP){
            return;
        }

        ActuatorCommand* newCommand = (ActuatorCommand*) inst.command;
        if (newCommand!= NULL && currentActuatorCommand!= NULL){
            //send error to old command
            debug("Newer command recived seding error to old one");
            sendResponseFromCommand(currentActuatorCommand, ERROR);
        }

        if (newCommand!=NULL){
            debug("executing new command");
            (this->*actuatorHandles[newCommand->getActuatorType()])(newCommand); // do new command ( map static cast )

        }
    }
}

void ExecutorCommon::stop(){
    commandQueueLock.lock();
    commandsToProcess.push(Instruction::STOP);
    commandQueueLock.unlock();
    queueNotEmpty.notify_one();
}




void ExecutorCommon::mapping(){
    debug("TODO: must redefine mapping");
}

void ExecutorCommon::suscribe(){
    debug("TODO: must redefine suscribe");
}

void ExecutorCommon::reloadConfig(ActuatorCommand* _command){
    debug("reload");
    bool success = false ;
    ReloadConfig* command = (ReloadConfig*) _command;
    currentActuatorCommand = command;
    debug("reload config with exec name: ");
    success = reload(&value, executorName);
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }

}

void ExecutorCommon::getPopcorn(ActuatorCommand* _command){
    bool success = false ;
    GetPopcorn* command = (GetPopcorn*) _command;
    currentActuatorCommand = command ;
    success = GetPopcornFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::unloadPopcorn(ActuatorCommand* _command){
    bool success = false ;
    UnloadPopcorn* command = (UnloadPopcorn*) _command;
    currentActuatorCommand = command ;
    success = UnloadPopcornFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::kickRight(ActuatorCommand* _command){
    bool success = false;
    KickRight* command = (KickRight*) _command;
    currentActuatorCommand = command;
    success = KickRightFunction();
    if (success){
        debug("sendin success");
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        debug("sending error");
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::kickLeft(ActuatorCommand* _command){
    bool success = false;
    KickLeft* command = (KickLeft*) _command;
    currentActuatorCommand = command;
    success = KickLeftFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::unKickRight(ActuatorCommand* _command){
    bool success = false;
    UnKickRight* command = (UnKickRight*) _command;
    currentActuatorCommand = command;
    success = UnKickRightFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::unKickLeft(ActuatorCommand* _command){
    bool success = false;
    UnKickLeft* command = (UnKickLeft*) _command;
    currentActuatorCommand = command;
    success = UnKickLeftFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}


void ExecutorCommon::getObject(ActuatorCommand * _command){
    bool success;
    GetObject* command = (GetObject*) _command;
    currentActuatorCommand = command;
    success =  GetObjectFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::unloadObject(ActuatorCommand* _command){
    bool success;
    UnloadObject *command  = (UnloadObject*) _command;
    currentActuatorCommand = command;
    success = UnloadObjectFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::getObjectStop(ActuatorCommand * _command){
    bool success;
    GetObjectStop* command = (GetObjectStop*) _command;
    currentActuatorCommand = command;
    success =  GetObjectStopFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::startBrxon(ActuatorCommand * _command){
    bool success;
    StartBrxon* command = (StartBrxon*) _command;
    currentActuatorCommand = command;
    success =  StartBrxonFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::startBeacon(ActuatorCommand * _command){
    bool success;
    StartBeacon* command = (StartBeacon*) _command;
    currentActuatorCommand = command;
    success =  StartBeaconFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}


void ExecutorCommon::stopBrxon(ActuatorCommand * _command){
    bool success;
    StopBrxon* command = (StopBrxon*) _command;
    currentActuatorCommand = command;
    success =  StopBrxonFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}


void ExecutorCommon::stopBeacon(ActuatorCommand * _command){
    bool success;
    StopBeacon* command = (StopBeacon*) _command;
    currentActuatorCommand = command;
    success =  StopBeaconFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::leaveCarpet(ActuatorCommand * _command){
    bool success;
    LeaveCarpet* command = (LeaveCarpet*) _command;
    currentActuatorCommand = command;
    success =  LeaveCarpetFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::callbackGetRight(ActuatorCommand * _command){
    bool success;
    CallbackGetRight* command = (CallbackGetRight*) _command;
    currentActuatorCommand = command;
    success =  CallbackGetRightFunction();
    if (success){
        //sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        //sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::callbackGetLeft(ActuatorCommand * _command){
    bool success;
    CallbackGetLeft* command = (CallbackGetLeft*) _command;
    currentActuatorCommand = command;
    success =  CallbackGetLeftFunction();
    if (success){
        //sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        //sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::startDetection(ActuatorCommand * _command){
    bool success;
    StartDetection* command = (StartDetection*) _command;
    currentActuatorCommand = command;
    success =  StartDetectionFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::stopDetection(ActuatorCommand * _command){
    bool success;
    StopDetection* command = (StopDetection*) _command;
    currentActuatorCommand = command;
    success =  StopDetectionFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

bool ExecutorCommon::KickRightFunction(){
    debug("KICK RIGHT: REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::KickLeftFunction(){
    debug("KICK LEFT: REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::UnKickRightFunction(){
    debug("UNKICK RIGHT: REDEFINE PLEASE");
     return false;
}

bool ExecutorCommon::UnKickLeftFunction(){
    debug("UNKICK LEFT: REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::GetPopcornFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::UnloadPopcornFunction(){
    debug("REDEFINE PLEASE");
    return false;
}
bool ExecutorCommon::UnloadObjectFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::GetObjectFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::SetSpeedFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::SetPositionFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::GetObjectStopFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::StopBrxonFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::StartBrxonFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::StartBeaconFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::StopBeaconFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::LeaveCarpetFunction(){
    debug("REDEFINE PLEASE");
    return false;
}




}
