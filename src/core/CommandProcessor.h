#ifndef _COMMANDPROCESSOR_H
#define _COMMANDPROCESSOR_H

#include <map>

#include "messages/Command.h"
#include "messages/CommandResponse.h"
#include "core/AbstractMessageHandler.h"

using namespace std;

namespace robot{

class CommandProcessor{
public:
    typedef void (CommandProcessor::*commandCallback)(Command* command);
    bool processCommand(Command*);
    void setHandler(AbstractMessageHandler* _handler);
protected:
    bool registerCommand(const string& commandName, commandCallback callback);
    bool sendResponse(CommandResponse* response);
    bool sendResponseFromCommand(Command* commandToRespond,ResponseStatus _status=SUCCESS);
private:
    map<string,commandCallback> registeredCommandCallbacks;
    AbstractMessageHandler* handler;
};

}

#endif //_COMMANDPROCESSOR_H
