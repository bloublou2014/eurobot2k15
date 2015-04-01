#ifndef _GETMOTIONSTATE_H
#define _GETMOTIONSTATE_H

#include "messages/Command.h"
#include "messages/CommandResponse.h"
#include "utils/helper/MotionState.h"

#include "drivers/motion/MotionDriver.h"

using namespace robot;
using geometry::Point2D;

namespace motion {

class GetMotionState:public Command{
public:
    static string NAME;

    GetMotionState():Command("GetMotionState", "MotionExecutor"){}
    GetMotionState(const GetMotionState& cmd):Command(cmd){}

    Message* clone(){
        return new GetMotionState(*this);
    }
private:
};

class GetMotionStateResponse : public CommandResponse{
public:
    GetMotionStateResponse(const string& to, const string& from, MotionState _state, ResponseStatus _status=SUCCESS):
        CommandResponse("GetMotionStateResponse",to,from,_status), state(_state) {}
    GetMotionStateResponse(const GetMotionStateResponse& cr):CommandResponse(cr),state(cr.state){}
    MotionState getState() const{return state;}

    Message* clone(){
        return new GetMotionStateResponse(*this);
    }
private:
    MotionState state;
};

}

#endif //_GETMOTIONSTATE_H
