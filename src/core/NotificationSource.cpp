#include "NotificationSource.h"

namespace robot{

bool NotificationSource::sendNotification(Notification* notification){
    if (handler!=NULL){
        return handler->receiveMessage(notification);
    }
    return false;
}

void NotificationSource::setHandler(AbstractMessageHandler* _handler){
    handler=_handler;
}

}
