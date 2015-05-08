#ifndef TASKMANAGERINTERFACE_H
#define TASKMANAGERINTERFACE_H

#include "core/AbstractMessageHandler.h"
#include "messages/StartMatchMessage.h"

#include <string>

using std::string;

namespace robot{

//lowest prority states have lowest values
enum TaskState{
    IMPOSSIBLE=0,
    FINISHED=1,
    SUSPENDED=2,
    READY=3,
    RUNNING=4
};

class TaskManagerInterface : public AbstractMessageHandler
{
public:
    virtual bool updateStatus(const string& taskName, TaskState newState)=0;
    virtual StartMessage::Color getMatchColor()=0;
    virtual bool isMatchStarted() const=0;

    virtual string getWorldProperty(const string& key)=0;
    virtual void setWorldProperty(const string& key, const string& value)=0;
};

}
#endif // TASKMANAGERINTERFACE_H
