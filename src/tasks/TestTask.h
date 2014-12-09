#ifndef _TESTTASK_H
#define _TESTTASK_H

#include "tasks/AbstractTask.h"
#include "executors/msg/TimePassed.h"
#include "executors/msg/CountdownCommand.h"

namespace robot{

class TestTask:public AbstractTask{
public:
    TestTask(const string& name):AbstractTask(name){}
protected:
    void initScript();
    void startScript();
    void stopScript();

    void init();
private:
    void handleTimePassNotification(Notification* notification);
    void handleMilanNotifications(Notification* testNotification);

    void countdownSuccess(CommandResponse* resp);
    void countdownProgress(CommandResponse* resp);
    void countdownError(CommandResponse* resp);
};

}

#endif //_TESTTASK_H
