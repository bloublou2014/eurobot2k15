#include "CommandProcessor.h"

namespace robot{

bool CommandProcessor::registerCommand(const string& commandName, commandCallback callback){
    registeredCommandCallbacks[commandName]=callback;
    return true;
}

bool CommandProcessor::processCommand(Command* command){
    if (registeredCommandCallbacks.find(command->getCommandName())!=registeredCommandCallbacks.end()){
        (this->*registeredCommandCallbacks[command->getCommandName()])(command);
        return true;
    }
    return false;
}

bool CommandProcessor::sendResponse(CommandResponse* response){
    //TODO: implement
    return true;
}

}
