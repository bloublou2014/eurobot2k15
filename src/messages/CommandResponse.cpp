#include "CommandResponse.h"

namespace robot {

string CommandResponse::getDestination() const{
    return destination;
}

ResponseStatus CommandResponse::getStatus() const{
    return status;
}

void CommandResponse::setId(int _id){
    id=_id;
}

int CommandResponse::getId() const{
    return id;
}

}
