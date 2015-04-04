#ifndef _COMMANDRESPONSE_H
#define _COMMANDRESPONSE_H


#include "Command.h"
#include "utils/javascript/JavaScriptMessageProvider.h"
#include "utils/javascript/ObjectWrap.h"
#include "utils/javascript/ObjectWrap.h"
#include "utils/javascript/JavaScriptMessageProvider.h"
#include "utils/javascript/ObjectWrap.h"

using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;

namespace robot {

enum ResponseStatus{
    SUCCESS,
    ERROR,
    PROGRESS_UPDATE
};

class CommandResponse: public Message{
public:
    static char* NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    CommandResponse(const string& _name, const string& _to, const string& _sender, ResponseStatus _status=SUCCESS):Message(COMMAND_RESPONSE, _sender),
        name(_name), destination(_to),status(_status),id(Command::INVALID_ID){
    }
    CommandResponse(const CommandResponse& cr):Message(cr),destination(cr.destination),status(cr.status),name(cr.name),id(cr.id){}

    string getDestination() const;
    ResponseStatus getStatus() const;
    void setId(int _id);
    int getId() const;

    string getName() const;

    Message* clone(){
        return new CommandResponse(*this);
    }

protected:
private:
    string destination;
    ResponseStatus status;
    string name;
    int id;
};

class DescribedResponse: public CommandResponse{
    static char* NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    static void DescriptionGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);

    DescribedResponse(const string& to, const string& from, ResponseStatus& response, const string& _description)
        :CommandResponse(NAME,to,from,response),description(_description){}
    DescribedResponse(const DescribedResponse& obj):CommandResponse(obj),description(obj.description){}
private:
    string description;
};

}

#endif //_COMMANDRESPONSE_H
