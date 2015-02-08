#ifndef JAVASCRIPTMESSAGE_H
#define JAVASCRIPTMESSAGE_H

#include <string>
#include <src/v8.h>

using std::string;
using namespace v8;

namespace javascript{

typedef Handle<ObjectTemplate> (*ObjectTemplateBuilder)(Isolate* isolate);
typedef Handle<FunctionTemplate> (*ConstructorTemplateBuilder)(Isolate* isolate);

class JavaScriptMessage
{
public:
    JavaScriptMessage(Isolate* isolate, ObjectTemplateBuilder objectBuilder, ConstructorTemplateBuilder constructorBuilder);

    Handle<FunctionTemplate> getConstructorTemplate();
    Handle<ObjectTemplate> getObjectTemplate();

    ObjectTemplateBuilder getObjectBuilder();
    ConstructorTemplateBuilder getConstructorBuilder();
    Handle<Function> createConstructorFunctoin(Isolate* isolate);
private:
    ObjectTemplateBuilder objectTemplateBuilder;
    ConstructorTemplateBuilder constructorTemplateBuilder;
    Persistent<FunctionTemplate> constructorTemplate;
    Persistent<ObjectTemplate> objectTemplate;
    string messageName;
    Isolate* isolate;
};

}
#endif // JAVASCRIPTMESSAGE_H
