#ifndef _COUNTDOWNCOMMAND_H
#define _COUNTDOWNCOMMAND_H

#include "messages/Command.h"
#include "messages/CommandResponse.h"

namespace robot {

class CountdownCommand:public Command{
public:
    CountdownCommand(const string& to, const string& from):Command("countToN",to, from){}
    void setCountdownValue(int _value);
    int getCountdownValue() const;
private:
    int value;
};

class CountdownCommandResponse : public CommandResponse{
public:
    CountdownCommandResponse(const string& to, const string& from, ResponseStatus _status=SUCCESS):CommandResponse(to,from,_status) {}
    int getValue() const {return value;}
    void setValue(int _value){value=_value;}
private:
    int value;
};

}

#endif //_COUNTDOWNCOMMAND_H
