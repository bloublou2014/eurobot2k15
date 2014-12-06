#ifndef _COMMANDRESPONSE_H
#define _COMMANDRESPONSE_H

#include "Message.h"

namespace robot {

enum ResponseStatus{
    SUCCESS,
    ERROR,
    PROGRESS_UPDATE
};

class CommandResponse:public Message{
public:
    CommandResponse(const string& _to, const string& _sender, ResponseStatus _status=SUCCESS):Message(COMMAND_RESPONSE, _sender),
        destination(_to),status(_status){}

    string getDestination() const;
    ResponseStatus getStatus() const;
protected:
private:
    string destination;
    ResponseStatus status;
};

}

#endif //_COMMANDRESPONSE_H
