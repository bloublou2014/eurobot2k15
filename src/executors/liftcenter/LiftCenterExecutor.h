#ifndef _LIFTCENTEREXECUTOR_H
#define _LIFTCENTEREXECUTOR_H

#include "executors/AbstractLiftExecutor.h"

namespace executor {

class LiftCenterExecutor: public AbstractLiftExecutor {
public:
    LiftCenterExecutor():AbstractLiftExecutor(this->NAME){}
    static string NAME;
    void suscribe();
    //bool GetObjectFunction();
    //bool UnloadObjectFunction();
private:


};


}


#endif // _LIFTCENTEREXECUTOR_H
