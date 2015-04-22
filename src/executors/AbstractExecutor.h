#ifndef _ABSTRACTEXECUTOR_H
#define _ABSTRACTEXECUTOR_H

#include "core/Node.h"
#include "core/NotificationHandler.h"
#include "core/CommandProcessor.h"
#include "core/NotificationSource.h"
#include "core/AbstractMessageHandler.h"

namespace robot{

    class AbstractExecutor: public Node, public NotificationHandler, public CommandProcessor, public NotificationSource
    {
        public:
            AbstractExecutor(const string& _name):Node(_name){}
            void registerManager(AbstractMessageHandler* manager);

            virtual void startMatch();
            void sendRawMessage(Message* msg);
        private:
            AbstractMessageHandler* handler;
    };

}

#endif //_ABSTRACTEXECUTOR_H
