#ifndef _NOTIFICATION_H
#define _NOTIFICATION_H

#include "Message.h"
#include "utils/javascript/ObjectWrap.h"
#include "utils/javascript/JavaScriptMessageProvider.h"
#include "utils/javascript/ObjectWrap.h"

using namespace std;

namespace robot{

class Notification : public Message{
public:
    Notification(const string& _topic, const string& _sender):Message(NOTIFICATION, _sender), topic(_topic){}
    Notification(const Notification& notification):Message(notification),topic(notification.topic){}

    string getTopic() const;

    string getName() const;
private:
    string topic;
};

}

#endif //_NOTIFICATION_H
