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
    GetLiftState(const GetLiftState& obj):Command(obj){}

    Message* clone(){
        return new GetLiftState(*this);
    }

private:
};

class GetLiftStateResponse : public CommandResponse{
public:
    GetLiftStateResponse(const string& _to, const string &_from, LiftState _state, ResponseStatus _status=SUCCESS ):
        CommandResponse("LiftExecutorGetLiftState",_to,_from,_status), state(_state){}
    GetLiftStateResponse(const GetLiftStateResponse& obj):CommandResponse(obj), state(obj.state){}
    LiftState getState()const{return state;}

    Message* clone(){
        return new GetLiftStateResponse(*this);
    }
private:
    LiftState state;
};

}




#endif // _GETLIFTSTATE_H
