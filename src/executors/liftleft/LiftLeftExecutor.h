#ifndef _LIFTLEFTEXECUTOR_H
#define _LIFTLEFTEXECUTOR_H

#include "executors/AbstractLiftExecutor.h"

#include "utils/helper/LiftState.h"
#include "executors/msg/GetLiftState.h"
#include "executors/msg/LiftNotification.h"

namespace executor {

class LiftLeftExecutor: public AbstractLiftExecutor {
public:
    LiftLeftExecutor():AbstractLiftExecutor(this->NAME){}
    static string NAME;
    void suscribe();
    bool GetObjectFunction();
    bool UnloadObjectFunction();
    void mapping();
    void processGetLiftState(Command* _command);
private:
   LiftState lastState;
   boost::mutex stateLock;
   bool sensorDetect = false;

};


}


#endif // _LIFTLEFTEXECUTOR_H
