#ifndef _ABSTRACTMESSAGEHANDLER_H
#define _ABSTRACTMESSAGEHANDLER_H

#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>

#include "messages/Message.h"

namespace robot {

class AbstractMessageHandler{
public:
    //Send message in system
    virtual bool sendMessage(Message* message)=0;
    //Receive message
    virtual bool receiveMessage(Message* message)=0;
protected:
};

class MessageHandlerUser{
public:
    virtual AbstractMessageHandler* getHandler()=0;
    virtual void setHandler(AbstractMessageHandler* _handler)=0;
};

}

#endif //_ABSTRACTMESSAGEHANDLER_H
