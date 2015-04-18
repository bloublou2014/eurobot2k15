#ifndef _CARPET_EXECUTOR
#define _CARPET_EXECUTOR

#include "executors/ExecutorCommon.h"
#include "drivers/actuators/ElectroSwitch.h"

using namespace electro_switch;
namespace executor{

class CarpetExecutor: public ExecutorCommon{
public:
    CarpetExecutor():ExecutorCommon(NAME){}
    static string NAME;
private:

    void suscribe();
    void mapping();
    void RelesCarpetFunction();
    ServoDriver armLeft;
    ServoDriver armRight;
    ElectroSwitch el_switchLeft;
    ElectroSwitch el_switchRight;
    bool LeaveCarpetFunction();

};


}


#endif // CARPET EXECUTOR

