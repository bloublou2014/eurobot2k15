#ifndef _COMMANDPROCESSOR_H
#define _COMMANDPROCESSOR_H

#include <map>

#include "messages/Command.h"

using namespace std;

namespace robot{

class CommandProcessor{
public:
    typedef void (CommandProcessor::*commandCallback)(Command* command);
    bool processCommand(Command*);
protected:
    bool registerCommand(const string& commandName, commandCallback callback);
private:
    map<string,commandCallback> registeredCommandCallbacks;
};

}

#endif //_COMMANDPROCESSOR_H
