#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include "utils/javascript/ObjectWrap.h"

using namespace std;
using namespace boost::posix_time;
using boost::shared_ptr;

namespace robot{

enum MessageType{
    NOTIFICATION,
    COMMAND,
    COMMAND_RESPONSE,
    STOP_MESSAGE
};

class Message : public javascript::ObjectWrap{
public:
    Message(MessageType _type, const string& _from):type(_type),source(_from){
        sentTime=second_clock::local_time();
    }

    Message(MessageType _type):type(_type){
        sentTime=second_clock::local_time();
    }

    MessageType getMessageType() const;
    string getSource() const;
    ptime getSentTime() const;

    void setSource(const string& _source);

    virtual string getName() const=0;

    virtual ~Message();

protected:

private:
    MessageType type;
    string source;
    ptime sentTime;
};

//typedef boost::shared_ptr<UMessage> Message;

}

#endif //_MESSAGE_H
