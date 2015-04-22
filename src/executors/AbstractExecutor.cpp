#include "AbstractExecutor.h"

namespace robot{

void AbstractExecutor::registerManager(AbstractMessageHandler* manager){
    CommandProcessor::setHandler(manager);
    NotificationSource::setHandler(manager);
    handler=manager;
}

void AbstractExecutor::startMatch(){
}

void AbstractExecutor::sendRawMessage(Message* msg){
    handler->receiveMessage(msg);
}

}
