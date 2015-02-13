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
    handler->receiveMessage(response);
    return true;
}

void CommandProcessor::setHandler(AbstractMessageHandler* _handler){
    handler=_handler;
}

bool CommandProcessor::sendResponseFromCommand(Command* commandToRespond,ResponseStatus _status){
    CommandResponse* resp=new CommandResponse(CommandResponse::NAME, commandToRespond->getSource(),commandToRespond->getDestination(),_status);
    resp->setId(commandToRespond->getId());
    return sendResponse(resp);
}

}
