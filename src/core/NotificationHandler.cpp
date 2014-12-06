#include "NotificationHandler.h"

namespace robot{

bool NotificationHandler::processNotification(Notification* notification){
    if (registeredNotificationCallbacks.find(notification->getTopic())!=registeredNotificationCallbacks.end()){
        (this->*registeredNotificationCallbacks[notification->getTopic()])(notification);
        return true;
    }
    return false;
}

bool NotificationHandler::subscribe(const string& notificationType, notificationCallback callback){
    registeredNotificationCallbacks[notificationType]=callback;
    return true;
}

bool NotificationHandler::unSubscribe(const string& notificationType){
    registeredNotificationCallbacks.erase(notificationType);
}

}
