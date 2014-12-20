#ifndef _COMMANDRESPONSE_H
#define _COMMANDRESPONSE_H

#include "Message.h"
#include "Command.h"

namespace robot {

enum ResponseStatus{
    SUCCESS,
    ERROR,
    PROGRESS_UPDATE
};

class CommandResponse:public Message{
public:
    CommandResponse(const string& _to, const string& _sender, ResponseStatus _status=SUCCESS):Message(COMMAND_RESPONSE, _sender),
        destination(_to),status(_status),id(Command::INVALID_ID){
    }

    string getDestination() const;
    ResponseStatus getStatus() const;
    void setId(int _id);
    int getId() const;
protected:
private:
    string destination;
    ResponseStatus status;
    int id;
};

}

#endif //_COMMANDRESPONSE_H
