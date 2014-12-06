#ifndef _COMMAND_H
#define _COMMAND_H

#include "Message.h"

namespace robot {

class Command:public Message{
public:
    static int INVALID_ID;  //-1

    Command(const string& _commandName, const string& _to, const string& _sender):Message(COMMAND, _sender),
        destination(_to), commandName(_commandName), id(INVALID_ID){}

    string getDestination() const;
    string getCommandName() const;
    void setId(int _id);
    int getId() const;
protected:
private:
    string destination;
    string commandName;
    int id;
};

}

#endif //_COMMAND_H
