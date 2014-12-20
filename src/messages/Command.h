#ifndef _COMMAND_H
#define _COMMAND_H

#include "Message.h"

namespace robot {

class Command:public UMessage{
public:
    static int INVALID_ID;  //-1

    Command(const string& _commandName, const string& _to, const string& _sender):UMessage(COMMAND, _sender),
        destination(_to), commandName(_commandName), id(INVALID_ID){}

    Command(const string& _commandName, const string& _to):UMessage(COMMAND),commandName(_commandName),id(INVALID_ID),
        destination(_to){}

    void setDestination(const string& _destination);
    void setId(int _id);

    string getDestination() const;
    string getCommandName() const;
    int getId() const;
protected:
private:
    string destination;
    string commandName;
    int id;
};

}

#endif //_COMMAND_H
