#ifndef _STOPMESSAGE_H
#define _STOPMESSAGE_H

#include "Message.h"

namespace robot {

class StopMessage:public Message{
public:
    StopMessage(const string& _sender):Message(STOP_MESSAGE,_sender){}
    StopMessage(const StopMessage& msg):Message(msg){}
    string getName() const{
        return "StopMessage";
    }

    Message* clone(){
        return new StopMessage(*this);
    }
protected:
private:
};

}
#endif  //_STOPMESSAGE_H
