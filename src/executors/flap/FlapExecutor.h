#ifndef _FLAPEXECUTOR_H
#define _FLAPEXECUTOR_H

#include "executors/ExecutorCommon.h"

namespace executor {

class FlapExecutor: public ExecutorCommon {
public:
    FlapExecutor():ExecutorCommon(this->NAME){}
    static string NAME;
    void suscribe();
    void mapping();

private:
    bool KickRightFunction();
    bool UnKickRightFunction();
    bool KickLeftFunction();
    bool UnKickLeftFunction();

    ServoDriver pipeLeft;
    ServoDriver pipeRight;


};


}



#endif // _FLAPEXECUTOR_H
