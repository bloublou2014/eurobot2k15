#ifndef STARTMATCHMESSAGE_H
#define STARTMATCHMESSAGE_H

#include "Message.h"

namespace robot {

class StartMessage:public Message{
public:
    StartMessage(const string& _sender):Message(START_MESSAGE,_sender){}
    StartMessage(const StartMessage& msg):Message(msg){}
    string getName() const{
        return "StartMessage";
    }

    Message* clone(){
        return new StartMessage(*this);
    }
};

}

#endif // STARTMATCHMESSAGE_H
