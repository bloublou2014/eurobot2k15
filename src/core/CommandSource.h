#ifndef _COMMANDSOURCE_H
#define _COMMANDSOURCE_H

#include <list>

#include "messages/Command.h"
#include "messages/CommandResponse.h"

using std::list;

namespace robot{

class CommandSource{
public:
    CommandSource():lastCommandId(0){}

    void processCommandResponse(CommandResponse* response);
protected:
    typedef void (CommandSource::*responseCallback)(CommandResponse* response);
    virtual bool sendCommand(Command* command, responseCallback success, responseCallback error, responseCallback progress=NULL);

    struct CallbackObject{
        CallbackObject(int,responseCallback,responseCallback,responseCallback);
        int commandId;
        responseCallback success;
        responseCallback error;
        responseCallback progress;
    };

private:
    list<CallbackObject> callbackObjects;
    int lastCommandId;
};

}


#endif  //_COMMANDSOURCE_H
