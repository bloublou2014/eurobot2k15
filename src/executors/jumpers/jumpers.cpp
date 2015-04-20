#include "jumpers.h"


namespace executor{

string JumpersExecutor::NAME = "JumpersExecutor";

void JumpersExecutor::init(){

    std::cout << " creating gpios" << std::endl;
    gpio_collor = new GPIOClass("18");
    gpio_start = new GPIOClass("22");

    gpio_collor->setdir_gpio("in");
    gpio_start->setdir_gpio("in");

}

void JumpersExecutor::main(){
    while(!shouldStop){
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        gpio_collor->getval_gpio(input_collor);
        gpio_start->getval_gpio(input_start);
        std::cout << "collor is: " << input_collor << std::endl;
        std::cout << "start is: " << input_start << std::endl;

    }
}

void JumpersExecutor::stop(){
    shouldStop = true;
}

} // end namespace
