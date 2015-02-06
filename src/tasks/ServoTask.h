#ifndef _SERVOTASK_H
#define _SERVOTASK_H

#include "tasks/AbstractTask.h"
#include "executors/msg/TimePassed.h"

#include "executors/servo/ServoTest/ServoCommand.h"


#include "executors/msg/MotionCommand.h"
#include "executors/msg/GetMotionState.h"

using namespace servo;

namespace robot{

class ServoTask:public AbstractTask{
public:
    ServoTask(const string& name):AbstractTask(name){}
protected:
    void initScript();
    void startScript();
    void stopScript();

    void init();
private:
    void handleMilanNotifications(Notification* testNotification);
    void handleMotionNotification(Notification* testNotification);

    void rotateFor(CommandResponse* resp);
    void roteteToPosition(CommandResponse* resp);
    void setSpeed(CommandResponse* resp);

    void servoError(CommandResponse* resp);

    int counter;

    //void moveForwardSuccess(CommandResponse* resp);
    //void rotateForSuccess(CommandResponse* resp);
    //void rotateToSuccess(CommandResponse* resp);
    //void moveArcSuccess(CommandResponse* resp);
    //void stopMovementSuccess(CommandResponse* resp);

    //void motionError(CommandResponse* resp);

    //void motionStateReceived(CommandResponse* resp);
};

}

#endif // _SERVOTASK_H
