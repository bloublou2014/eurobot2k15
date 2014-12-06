#include "Command.h"

namespace robot {

string Command::getDestination() const{
    return destination;
}

string Command::getCommandName() const{
    return commandName;
}

}
