#include "CommandResponse.h"

namespace robot {

string CommandResponse::getDestination() const{
    return destination;
}

ResponseStatus CommandResponse::getStatus() const{
    return status;
}

}
