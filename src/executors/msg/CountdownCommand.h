#ifndef _COUNTDOWNCOMMAND_H
#define _COUNTDOWNCOMMAND_H

#include <include/v8.h>

#include "messages/Command.h"
#include "messages/CommandResponse.h"
#include "utils/javascript/JavaScriptMessageProvider.h"

using namespace v8;
using javascript::JavaScriptMessage;
using javascript::JavaScriptMessageProvider;

namespace robot {

class CountdownCommand:public Command{
public:
    CountdownCommand(int _value):Command("countToN","ExampleExecutor"),value(_value){}
    void setCountdownValue(int _value);
    int getCountdownValue() const;

    /* For object instance */
    static Handle<ObjectTemplate> CreateTemplate(Isolate* isolate);
    static void ValueGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void ValueSetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info);

    /* For constructor */
    static Handle<FunctionTemplate> CreateConstructorTemplate(Isolate* isolate);
    static void constructorCall(const v8::FunctionCallbackInfo<Value> &args);
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
