#ifndef JAVASCRIPTMESSAGEPROVIDER_H
#define JAVASCRIPTMESSAGEPROVIDER_H

#include <string>
#include <src/v8.h>

using std::string;
using namespace v8;

namespace javascript{

class JavaScriptMessageProvider
{
public:
    virtual Handle<Function> getObjectConstructor(const string& name)=0;
    virtual void setObjectConstructor(const string &name, Handle<Function> newTemplate)=0;
    virtual Handle<Object> wrapObject(const string& name, Isolate* isolate, void* ptr)=0;
    virtual void setObjectTemplate(const string& name, Handle<ObjectTemplate> tmpl)=0;
};

}

#endif // JAVASCRIPTMESSAGEPROVIDER_H
