#include "AbstractExecutor.h"

namespace robot{

void AbstractExecutor::registerManager(AbstractMessageHandler* manager){
    CommandProcessor::setHandler(manager);
    NotificationSource::setHandler(manager);
}

void AbstractExecutor::startMatch() const{
}

void AbstractExecutor::sendMessage(Message* msg){
    handler->receiveMessage(msg);
}

}
