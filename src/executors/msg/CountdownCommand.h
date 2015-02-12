#ifndef _COUNTDOWNCOMMAND_H
#define _COUNTDOWNCOMMAND_H

#include <include/v8.h>
#include "utils/javascript/ObjectWrap.h"

#include "messages/Command.h"
#include "messages/CommandResponse.h"
#include "utils/javascript/JavaScriptMessageProvider.h"

using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;

namespace robot {

class CountdownCommand:public Command{
public:
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    /* Getters and setters */
    static void ValueGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void ValueSetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info);

    CountdownCommand(int _value):Command("countToN","ExampleExecutor"),value(_value){}
    void setCountdownValue(int _value);
    int getCountdownValue() const;

    string getName() const;
private:
    int value;
};

class CountdownCommandResponse : public CommandResponse{
public:
    CountdownCommandResponse(const string& to, const string& from, ResponseStatus _status=SUCCESS):CommandResponse("CountdownCommandResponse", to,from,_status) {}
    int getValue() const {return value;}
    void setValue(int _value){value=_value;}
private:
    int value;
};

}

#endif //_COUNTDOWNCOMMAND_H
