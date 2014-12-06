#ifndef _COMMAND_H
#define _COMMAND_H

#include "Message.h"

namespace robot {

class Command:public Message{
public:
    Command(const string& _commandName, const string& _to, const string& _sender):Message(COMMAND, _sender),
        destination(_to), commandName(_commandName){}

    string getDestination() const;
    string getCommandName() const;
protected:
private:
    string destination;
    string commandName;
};

}

#endif //_COMMAND_H
