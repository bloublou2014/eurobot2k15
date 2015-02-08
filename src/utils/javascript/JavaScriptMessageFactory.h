#ifndef JAVASCRIPTMESSAGEFACTORY_H
#define JAVASCRIPTMESSAGEFACTORY_H

#include <string>
#include <map>
#include <src/v8.h>
#include "utils/javascript/JavaScriptMessageProvider.h"

#include "executors/msg/CountdownCommand.h"

using std::string;
using std::map;
using namespace v8;

using robot::CountdownCommand;

namespace javascript{

class JavaScriptMessageFactory : public JavaScriptMessageProvider
{
public:
    JavaScriptMessageFactory(){}

    void init(Handle<Object> exportObject);

    Handle<Function> getObjectConstructor(const string& name);
    void setObjectConstructor(const string &name, Handle<Function> newTemplate);
private:
    map<string, Persistent<Function>> registeredConstructors;
};

}
#endif // JAVASCRIPTMESSAGEFACTORY_H
