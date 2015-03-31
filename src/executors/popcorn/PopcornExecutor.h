#ifndef _POPCORNEXECUTOR_H
#define _POPCORNEXECUTOR_H

#include "executors/ExecutorCommon.h"

namespace executor {

class PopcornExecutor: public ExecutorCommon {
public:
    PopcornExecutor():ExecutorCommon(this->NAME){}
    static string NAME;
    void suscribe();
    void mapping();
private:
    bool GetPopcornFunction();
    bool UnloadPopcornFunction();

    ServoDriver pipeLeft;
    ServoDriver pipeRight;
    ServoDriver backDoor;

};


}


#endif // _POPCORNEXECUTOR_H
