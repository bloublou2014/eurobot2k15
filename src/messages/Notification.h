#ifndef _NOTIFICATION_H
#define _NOTIFICATION_H

#include "Message.h"

using namespace std;

namespace robot{

class Notification : public UMessage {
public:
    Notification(const string& _topic, const string& _sender):UMessage(NOTIFICATION, _sender), topic(_topic){}
    string getTopic() const;
private:
    string topic;
};

}

#endif //_NOTIFICATION_H
