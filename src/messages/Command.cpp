#include "Command.h"

namespace robot {

int Command::INVALID_ID=-1;

string Command::getDestination() const{
    return destination;
}

string Command::getCommandName() const{
    return commandName;
}

void Command::setId(int _id){
    id=_id;
}

int Command::getId() const{
    return id;
}

}
