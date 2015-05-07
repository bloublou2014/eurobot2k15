#ifndef JAVASCRIPTVM_H
#define JAVASCRIPTVM_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

#include <include/v8.h>
#include <include/libplatform/libplatform.h>

#include "core/Logger.h"
#include "utils/javascript/JavaScriptMessageFactory.h"

using namespace v8;
using javascript::JavaScriptMessageFactory;
using robot::Logger;

namespace robot{

struct JavaScriptVMException: public std::exception{
    JavaScriptVMException(const string& _reason):reason(_reason){}

    const char* what() const throw (){
        return reason.c_str();
    }

private:
    string reason;
};

class JavaScriptVM
{
public:
    JavaScriptVM();

    static void InitV8Platform();
protected:
    bool executeGlobalFunction(Handle<Function> function, const int argc, Handle<Value> argv[]);
    static void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch);
    static const char* ToCString(const v8::String::Utf8Value& value);
    static Handle<String> ReadScript(Isolate* isolate, const string& fileName);

    //Helper functions for exposing objects in javascript
    typedef Handle<ObjectTemplate> (*ObjectTemplateBuilder)(Isolate* isolate);

    Handle<Object> createObjectFromTemplate(ObjectTemplateBuilder builder, Persistent<ObjectTemplate>& objTemplate, void* internalField);

//    template <class T>
//    static T* UnwrapObject(Handle<Object> object){
//        Handle<External> field = Handle<External>::Cast(object->GetInternalField(0));
//        void* ptr = field->Value();
//        return static_cast<T*>(ptr);
//    }

    Handle<Script> compileScript(Handle<String> scriptSource);
    void runScript(Handle<Script> compiledScript);

    inline Isolate* getIsolate() const{
        return isolate;
    }

    JavaScriptMessageFactory* messageFactory;
    Persistent<Context> taskContext;

    void initializeGlobalState();
    void finalize();

    virtual void createGlobalObjects(Handle<Object> globalObj)=0;
    virtual void getScriptNames(std::vector<string>& scripts)=0;
private:
    Isolate* isolate;
};

}

#endif // JAVASCRIPTVM_H
