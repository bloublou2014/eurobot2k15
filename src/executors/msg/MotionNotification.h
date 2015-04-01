#ifndef _MOTIONNOTIFICATION_H
#define _MOTIONNOTIFICATION_H

#include "messages/Notification.h"
#include "utils/helper/MotionState.h"

using namespace robot;

namespace motion{

class MotionNotification : public Notification{
public:
    MotionNotification(MotionState _state):Notification("MotionNotification","MotionExecutor"),state(_state){}
    MotionNotification(const MotionNotification& mn):Notification(mn),state(mn.state){}
    MotionState getState() const{ return state; }

    Message* clone(){
        return new MotionNotification(*this);
    }

private:
    MotionState state;
};

}

#endif //_MOTIONNOTIFICATION_H
