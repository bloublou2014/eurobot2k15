#ifndef _JUMPERS
#define _JUMPERS

#include "executors/AbstractExecutor.h"
#include "utils/gpio/gpio.h"

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

    GPIOClass* gpio_start;
    GPIOClass* gpio_collor;

    std::string input_start;
    std::string input_collor;

    bool shouldStop = false;
};

} // end namespace

#endif
