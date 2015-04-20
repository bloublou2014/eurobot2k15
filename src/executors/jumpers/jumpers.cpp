#include "jumpers.h"


namespace executor{

string JumpersExecutor::NAME = "JumpersExecutor";

void JumpersExecutor::init(){


}

void JumpersExecutor::main(){
    while(!shouldStop){

    }
}

void JumpersExecutor::stop(){
    shouldStop = true;
}

} // end namespace
