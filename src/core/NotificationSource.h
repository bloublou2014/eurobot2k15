#ifndef _NOTIFICATIONSOURCE_H
#define _NOTIFICATIONSOURCE_H

#include "messages/Notification.h"
#include "AbstractMessageHandler.h"

namespace robot{

class NotificationSource{
public:
    NotificationSource(){}
    void setHandler(AbstractMessageHandler* _handler);
protected:
    bool sendNotification(Notification* notification);
    AbstractMessageHandler* handler;
};

}

#endif  //_NOTIFICATIONSOURCE_H
