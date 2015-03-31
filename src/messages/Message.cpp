#include "Message.h"

namespace robot {

MessageType Message::getMessageType() const{
    return type;
}

string Message::getSource() const{
    return source;
}

ptime Message::getSentTime() const{
    return sentTime;
}

void Message::setSource(const string& _source){
    source=_source;
}

Message::~Message(){

}

}
