#ifndef _MOTIONNOTIFICATION_H
#define _MOTIONNOTIFICATION_H

#include "messages/Notification.h"
#include "utils/helper/MotionState.h"

using namespace robot;

namespace motion{

class MotionNotification : public Notification{
public:
    MotionNotification(MotionState _state):Notification("MotionNotification","MotionExecutor"),state(_state){}
    MotionState getState() const{ return state; }
private:
    MotionState state;
};

}

#endif //_MOTIONNOTIFICATION_H
