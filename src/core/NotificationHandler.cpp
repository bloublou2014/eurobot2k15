#include "NotificationHandler.h"

namespace robot{

bool NotificationHandler::isSubscribed(Notification* notification){
    boost::shared_lock<shared_mutex> lock(mapLock);
    return registeredNotificationCallbacks.find(notification->getTopic())!=registeredNotificationCallbacks.end();
}

bool NotificationHandler::processNotification(Notification* notification){
    boost::shared_lock<shared_mutex> lock(mapLock);
    if (registeredNotificationCallbacks.find(notification->getTopic())!=registeredNotificationCallbacks.end()){
        (this->*registeredNotificationCallbacks[notification->getTopic()])(notification);
        return true;
    }
    return false;
}

bool NotificationHandler::subscribe(const string& notificationType, notificationCallback callback){
    boost::unique_lock<shared_mutex> uniqueLock(mapLock);
    registeredNotificationCallbacks[notificationType]=callback;
    return true;
}

bool NotificationHandler::unSubscribe(const string& notificationType){
    boost::unique_lock<shared_mutex> uniqueLock(mapLock);
    registeredNotificationCallbacks.erase(notificationType);
    return true;
}

}
