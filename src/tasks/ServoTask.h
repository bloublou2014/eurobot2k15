#ifndef _SERVOTASK_H
#define _SERVOTASK_H

#include "tasks/AbstractTask.h"
#include "executors/msg/TimePassed.h"

#include "executors/servo/ServoTest/ServoCommand.h"


#include "executors/msg/MotionCommand.h"
#include "executors/msg/GetMotionState.h"

#include "executors/msg/ActuatorCommand.h"
#include "executors/msg/GetLiftState.h"
#include "executors/msg/LIftNotification.h"

#include "utils/helper/LiftState.h"

#include "executors/liftright/LiftRightExecutor.h"

using namespace servo;
using namespace executor;

namespace robot{

class ServoTask:public AbstractTask{
public:
    ServoTask(const string& name):AbstractTask(name){}
protected:
    void initScript();
    void startScript();
    void stopScript();
    void killTask();
    void init();
private:
    void handleMilanNotifications(Notification* testNotification);
    void handleLiftNotification(Notification* testNotification);
    void setState(Notification *resp);
    void liftStateRecive(CommandResponse *resp);

    void rotateFor(CommandResponse* resp);
    void roteteToPosition(CommandResponse* resp);
    void setSpeed(CommandResponse* resp);

    void servoError(CommandResponse* resp);

    int counter = 0 ;
    bool shouldStop = false ;

    LiftState state;

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
