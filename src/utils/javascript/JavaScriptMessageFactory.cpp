#include "JavaScriptMessageFactory.h"

namespace javascript{

JavaScriptMessageFactory::JavaScriptMessageFactory(Isolate *isolate){
    registeredMessages["CountdownCommand"]=new JavaScriptMessage(isolate, CountdownCommand::CreateTemplate,CountdownCommand::CreateConstructorTemplate);
}

JavaScriptMessageFactory::~JavaScriptMessageFactory(){
    for (map<string, JavaScriptMessage*>::iterator it=registeredMessages.begin();it!=registeredMessages.end();++it){
        delete it->second;
    }
}

JavaScriptMessage* JavaScriptMessageFactory::getMessageHandler(const string& name){
    return registeredMessages[name];
}

}
