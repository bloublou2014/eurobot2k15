#ifndef _TESTTASK_H
#define _TESTTASK_H

#include "tasks/AbstractTask.h"

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

    void handleTestNotifications(Notification* testNotification);
};

}

#endif //_TESTTASK_H
