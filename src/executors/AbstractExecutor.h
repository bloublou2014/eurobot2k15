#ifndef _ABSTRACTEXECUTOR_H
#define _ABSTRACTEXECUTOR_H

#include "core/Node.h"
#include "core/NotificationHandler.h"
#include "core/CommandProcessor.h"

namespace robot{

class AbstractExecutor: public Node, public NotificationHandler, public CommandProcessor{
public:
    AbstractExecutor(const string& _name):Node(string("_name")){}
    void processTestEvent(Notification* notification);
};

}

#endif //_ABSTRACTEXECUTOR_H
