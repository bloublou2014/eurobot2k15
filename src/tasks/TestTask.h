#ifndef _TESTTASK_H
#define _TESTTASK_H

#include "tasks/AbstractTask.h"
#include "executors/msg/TimePassed.h"

#include "executors/msg/MotionCommand.h"

using namespace motion;

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
    void handleMilanNotifications(Notification* testNotification);

    void moveForwardSuccess(CommandResponse* resp);
    void rotateForSuccess(CommandResponse* resp);
    void rotateToSuccess(CommandResponse* resp);
    void moveArcSuccess(CommandResponse* resp);
    void stopMovementSuccess(CommandResponse* resp);

    void motionError(CommandResponse* resp);
};

}

#endif //_TESTTASK_H
