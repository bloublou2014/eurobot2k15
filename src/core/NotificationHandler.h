#ifndef _NOTIFICATIONHANDLER_H
#define _NOTIFICATIONHANDLER_H

#include <exception>
#include <map>
#include <boost/thread/shared_mutex.hpp>

#include "messages/Notification.h"

using namespace std;
using boost::shared_mutex;

namespace robot{

class NotificationHandler{
public:
    typedef void (NotificationHandler::*notificationCallback)(Notification* notification);
    bool isSubscribed(Notification* notification);
    virtual bool processNotification(Notification* notification);
protected:
    bool subscribe(const string& notificationType, notificationCallback callback);
    bool unSubscribe(const string& notificationType);
private:
    shared_mutex mapLock;
    map<string,notificationCallback> registeredNotificationCallbacks;
};

struct NotificationProcessingException: public std::exception{
    NotificationProcessingException(const string& _reason):reason(_reason){}

    const char* what() const throw (){
        return reason.c_str();
    }

private:
    string reason;
};

}

#endif //_NOTIFICATIONHANDLER_H
