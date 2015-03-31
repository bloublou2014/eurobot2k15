#ifndef _GETLIFTSTATE_H
#define _GETLIFTSTATE_H

#include "messages/Command.h"
#include "messages/CommandResponse.h"

#include "utils/helper/LiftState.h"


using namespace  robot; // za command

namespace executor {

class GetLiftState: public Command{
public:
    static string NAME;

    GetLiftState(string _liftExecutorName):Command(NAME, _liftExecutorName){}

private:
};

class GetLiftStateResponse : public CommandResponse{
public:
    GetLiftStateResponse(const string& _to, const string &_from, LiftState _state, ResponseStatus _status=SUCCESS ):
        CommandResponse(_to,_from,_status), state(_state){}

    LiftState getState()const{return state;}

private:
    LiftState state;
};

}




#endif // _GETLIFTSTATE_H
