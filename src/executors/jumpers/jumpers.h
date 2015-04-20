#ifndef _JUMPERS
#define _JUMPERS

#include "executors/AbstractExecutor.h"
#include <wiringPi.h>


using namespace robot;
namespace executor{

class JumpersExecutor : public AbstractExecutor {
public:
    static string NAME;
    JumpersExecutor():AbstractExecutor(this->NAME){}
private:
    void stop();
    void main();
    void init();

    bool shouldStop = false;
};

} // end namespace

#endif
