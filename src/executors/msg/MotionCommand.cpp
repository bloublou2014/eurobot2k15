#include "CountdownCommand.h"

namespace robot {

void CountdownCommand::setCountdownValue(int _value){
    value=_value;
}

int CountdownCommand::getCountdownValue() const{
    return value;
}

}
