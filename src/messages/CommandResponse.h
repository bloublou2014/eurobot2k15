#ifndef _COMMANDRESPONSE_H
#define _COMMANDRESPONSE_H

#include "Command.h"

namespace robot {

enum ResponseStatus{
    SUCCESS,
    ERROR,
    PROGRESS_UPDATE
};

class CommandResponse:public Message{
public:
    CommandResponse(const string& _name, const string& _to, const string& _sender, ResponseStatus _status=SUCCESS):Message(COMMAND_RESPONSE, _sender),
        name(_name), destination(_to),status(_status),id(Command::INVALID_ID){
    }

    string getDestination() const;
    ResponseStatus getStatus() const;
    void setId(int _id);
    int getId() const;

    string getName() const;
protected:
private:
    string destination;
    ResponseStatus status;
    string name;
    int id;
};

}

#endif //_COMMANDRESPONSE_H
