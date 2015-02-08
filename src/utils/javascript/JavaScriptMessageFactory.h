#ifndef JAVASCRIPTMESSAGEFACTORY_H
#define JAVASCRIPTMESSAGEFACTORY_H

#include <string>
#include <map>
#include <src/v8.h>
#include "utils/javascript/JavaScriptMessage.h"
#include "utils/javascript/JavaScriptMessageProvider.h"

#include "executors/msg/CountdownCommand.h"

using std::string;
using std::map;
using namespace v8;
/*Messages*/
using robot::CountdownCommand;

namespace javascript{

class JavaScriptMessageFactory : public JavaScriptMessageProvider
{
public:
    JavaScriptMessageFactory(Isolate* isolate);
    ~JavaScriptMessageFactory();

    JavaScriptMessage* getMessageHandler(const string& name);


private:
    map<string, JavaScriptMessage*> registeredMessages;
};

}
#endif // JAVASCRIPTMESSAGEFACTORY_H
