#ifndef _NOTIFICATIONHANDLER_H
#define _NOTIFICATIONHANDLER_H

#include <map>

#include "messages/Notification.h"

using namespace std;

namespace robot{

class NotificationHandler{
public:
    typedef void (NotificationHandler::*notificationCallback)(Notification* notification);

    bool processNotification(Notification* notification);
protected:
    bool subscribe(const string& notificationType, notificationCallback callback);
    bool unSubscribe(const string& notificationType);
private:
    map<string,notificationCallback> registeredNotificationCallbacks;
};

}


#endif //_NOTIFICATIONHANDLER_H
