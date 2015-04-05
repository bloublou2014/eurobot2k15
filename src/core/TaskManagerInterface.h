#ifndef TASKMANAGERINTERFACE_H
#define TASKMANAGERINTERFACE_H

#include "core/AbstractMessageHandler.h"
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
};

}
#endif // TASKMANAGERINTERFACE_H
