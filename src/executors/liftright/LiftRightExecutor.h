#ifndef _LIFTRIGHTEXECUTOR_H
#define _LIFTRIGHTEXECUTOR_H

#include "executors/AbstractLiftExecutor.h"

#include "utils/helper/LiftState.h"
#include "executors/msg/GetLiftState.h"
#include "executors/msg/LiftNotification.h"

namespace executor {

class LiftRightExecutor: public AbstractLiftExecutor {
public:
    LiftRightExecutor():AbstractLiftExecutor(this->NAME){}
    static string NAME;
    void suscribe();
    bool GetObjectFunction();
    bool UnloadObjectFunction();
    void brodcastNotification();
    void mapping();
private:
    void processGetLiftState(Command* _command);
    LiftState lastState;
    boost::mutex stateLock;

};


}


#endif // _LIFTRIGHTEXECUTOR_H
