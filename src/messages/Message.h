#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
using namespace boost::posix_time;

namespace robot{

enum MessageType{
    NOTIFICATION,
    COMMAND,
    COMMAND_RESPONSE,
    STOP_MESSAGE
};

class Message{
public:
    Message(MessageType _type, const string& _from):type(_type),source(_from){
        sentTime=second_clock::local_time();
    }

    MessageType getMessageType() const;
    string getSource() const;
    ptime getSentTime() const;
protected:

private:
    MessageType type;
    string source;
    ptime sentTime;
};

}

#endif //_MESSAGE_H
