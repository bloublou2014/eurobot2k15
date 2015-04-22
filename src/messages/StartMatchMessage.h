#ifndef STARTMATCHMESSAGE_H
#define STARTMATCHMESSAGE_H

#include "Message.h"

namespace robot {

class StartMessage:public Message{
public:
    enum Color{
        YELLOW=1,
        GREEN=2
    };

    StartMessage(Color _color, const string& _sender):Message(START_MESSAGE,_sender),
        color(_color){}
    StartMessage(const StartMessage& msg):Message(msg), color(msg.color){}
    inline Color getColor() const { return color; }

    string getName() const{
        return "StartMessage";
    }

    Message* clone(){
        return new StartMessage(*this);
    }

private:
    Color color;
};

}

#endif // STARTMATCHMESSAGE_H
