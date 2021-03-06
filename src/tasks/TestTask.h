#ifndef _TESTTASK_H
#define _TESTTASK_H

#include "tasks/AbstractTask.h"
#include "executors/msg/TimePassed.h"

#include "executors/msg/MotionCommand.h"
#include "executors/msg/GetMotionState.h"

using namespace motion;

namespace robot{

class TestTask:public AbstractTask{
public:
    TestTask(const string& name):AbstractTask(name){}
protected:
    void onCreate();
    void onRun();
    void onPause();
    void onDestroy();

    void init();
private:
    void handleMilanNotifications(Notification* testNotification);
    void handleMotionNotification(Notification* testNotification);

    void moveForwardSuccess(CommandResponse* resp);
    void rotateForSuccess(CommandResponse* resp);
    void rotateToSuccess(CommandResponse* resp);
    void moveArcSuccess(CommandResponse* resp);
    void stopMovementSuccess(CommandResponse* resp);

    void motionError(CommandResponse* resp);

    void motionStateReceived(CommandResponse* resp);
};

}

#endif //_TESTTASK_H
