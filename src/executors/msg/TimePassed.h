#ifndef _TIMEPASSED_H
#define _TIMEPASSED_H

#include "messages/Notification.h"

namespace robot{

class TimePassedNotification : public Notification{
public:
    TimePassedNotification(const string& sender,int time):Notification("timePassedNotification",sender),passedTime(time){}
    int getPassedTime() const;
protected:
private:
    int passedTime;
};

}

#endif //_TIMEPASSED_H
