#ifndef _CARPET_EXECUTOR
#define _CARPET_EXECUTOR

#include "executors/ExecutorCommon.h"

namespace executor{

class CarpetExecutor: public ExecutorCommon{
public:
    CarpetExecutor():ExecutorCommon(NAME){}
    static string NAME;
private:

    void suscribe();
    void mapping();
    void RelesCarpetFunction();

};


}


#endif // CARPET EXECUTOR

