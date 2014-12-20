#ifndef _MOTIONCOMMAND_H
#define _MOTIONCOMMAND_H

#include "messages/Command.h"
#include "messages/CommandResponse.h"

namespace robot {

class MotionCommand:public Command{
public:
    MotionCommand(const string& to, const string& from):Command("MotionCommand",to, from){}
private:
};

class MotionCommandResponse : public CommandResponse{
public:
    MotionCommandResponse(const string& to, const string& from, ResponseStatus _status=SUCCESS):CommandResponse(to,from,_status) {}
private:
};

}

#endif //_MOTIONCOMMAND_H
