#ifndef _LIFTNOTIFICATION_
#define _LIFTNOTIFICATION_

#include "messages/Notification.h"
#include "utils/helper/LiftState.h"

using namespace robot;

namespace executor {

class LiftNotification: public Notification {
public:
    LiftNotification(LiftState _state):Notification(notificationNAME, "LiftRightExecutor" ), state(_state){}

    LiftState getState() const { return state; }

    static std::string notificationNAME;
private :
    LiftState state;

};

} // end namespace

#endif // _LIFTNOTIFICATION_
