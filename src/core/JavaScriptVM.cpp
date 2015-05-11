#include "JavaScriptVM.h"

namespace robot{

JavaScriptVM::JavaScriptVM(){
}

void JavaScriptVM::InitV8Platform(){
    v8::V8::InitializeICU();
    v8::Platform* platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);
    v8::V8::Initialize();
}

void JavaScriptVM::ReportException(Isolate *isolate, TryCatch *try_catch){
    static mutex exceptionLock;
    unique_lock<mutex> lock(exceptionLock);

    v8::HandleScope handle_scope(isolate);
    v8::String::Utf8Value exception(try_catch->Exception());
    const char* exception_string = ToCString(exception);
    v8::Handle<v8::Message> message = try_catch->Message();
    if (message.IsEmpty()) {
        // V8 didn't provide any extra information about this error; just
        // print the exception.
        fprintf(stderr, "%s\n", exception_string);
    } else {
        // Print (filename):(line number): (message).
        v8::String::Utf8Value filename(message->GetScriptOrigin().ResourceName());
        const char* filename_string = ToCString(filename);
        int linenum = message->GetLineNumber();
        fprintf(stderr, "%s:%i: %s\n", filename_string, linenum, exception_string);
        // Print line of source code.
        v8::String::Utf8Value sourceline(message->GetSourceLine());
        const char* sourceline_string = ToCString(sourceline);
        fprintf(stderr, "%s\n", sourceline_string);
        // Print wavy underline (GetUnderline is deprecated).
        int start = message->GetStartColumn();
        for (int i = 0; i < start; i++) {
            fprintf(stderr, " ");
        }
        int end = message->GetEndColumn();
        for (int i = start; i < end; i++) {
            fprintf(stderr, "^");
        }
        fprintf(stderr, "\n");
        v8::String::Utf8Value stack_trace(try_catch->StackTrace());
        if (stack_trace.length() > 0) {
            const char* stack_trace_string = ToCString(stack_trace);
            fprintf(stderr, "%s\n", stack_trace_string);
        }
    }
}

const char *JavaScriptVM::ToCString(const String::Utf8Value &value){
    return *value ? *value : "<string conversion failed>";
}

Handle<String> JavaScriptVM::ReadScript(Isolate *isolate, const string &fileName){
    FILE* file = fopen(fileName.c_str(), "rb");
    if (file == NULL) return Handle<String>();

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    char* chars = new char[size + 1];
    chars[size] = '\0';

    for (int i = 0; i < size;) {
        int read = static_cast<int>(fread(&chars[i], 1, size - i, file));
        i += read;
    }

    fclose(file);
    Handle<String> result = String::NewFromUtf8(isolate, chars, String::kNormalString, size);
    delete[] chars;
    return result;
}

Handle<Object> JavaScriptVM::createObjectFromTemplate(JavaScriptVM::ObjectTemplateBuilder builder, Persistent<ObjectTemplate> &objTemplate, void *internalField){
    EscapableHandleScope scope(getIsolate());

    if (objTemplate.IsEmpty()){
        Handle<ObjectTemplate> newTemplate= builder(getIsolate());
        objTemplate.Reset(getIsolate(), newTemplate);
    }

    Handle<ObjectTemplate> templ=Local<ObjectTemplate>::New(getIsolate(), objTemplate);
    Local<Object> result=templ->NewInstance();
    Handle<External> refferencePtr=External::New(getIsolate(), internalField);
    result->SetInternalField(0, refferencePtr);

    return scope.Escape(result);
}

void JavaScriptVM::callJavascriptCommandCallback(Persistent<Function>& function, CommandResponse* resp){
    HandleScope scope(getIsolate());
    Local<Context> context =Local<Context>::New(getIsolate(), taskContext);
    Context::Scope contextScope(context);

    TryCatch tryCatch;

    Handle<Object> obj;
    int argc=1;
    Handle<Value> argv[argc];
    if (resp!=NULL){
        obj=messageFactory->wrapObject(resp->getName(),getIsolate(),resp);
        argc=1;
        argv[0]=obj;
    }else{
        argc=0;
    }

    Local<Function> func=Local<Function>::New(getIsolate(), function);
    Handle<Value> result= func->Call(context->Global(),argc, argv);
    if (result.IsEmpty()){
        ReportException(getIsolate(), &tryCatch);
        throw JavaScriptVMException("Running callback method function failed.");
    }
}

Handle<Script> JavaScriptVM::compileScript(Handle<String> scriptSource){
    EscapableHandleScope scope(getIsolate());

    TryCatch tryCatch;
    Local<Script> compiledScript=Script::Compile(scriptSource);
    if (compiledScript.IsEmpty()){
        ReportException(getIsolate(), &tryCatch);
        throw JavaScriptVMException("Compiling script contains errors. Check stack trace for details.");
    }
    return scope.Escape(compiledScript);
}

void JavaScriptVM::runScript(Handle<Script> compiledScript){
    TryCatch tryCatch;
    Local<Value> result=compiledScript->Run();
    if (result.IsEmpty()){
        ReportException(getIsolate(), &tryCatch);
        throw JavaScriptVMException("Running script contains errors. Check stack trace for details.");
    }
}

void JavaScriptVM::initializeGlobalState(){
    isolate = Isolate::New();
    isolate->Enter();

    HandleScope scope(getIsolate());

    Handle<Context> context=Context::New(getIsolate());
    taskContext.Reset(getIsolate(), context);

    Context::Scope contextScope(context);

    messageFactory=new JavaScriptMessageFactory();
    isolate->SetData(0,messageFactory);

    Handle<Object> global=context->Global();
    createGlobalObjects(global);
    messageFactory->init(global);

    vector<string> scripts;
    getScriptNames(scripts);
    vector<string>::iterator it=scripts.begin();
    for(;it!=scripts.end();++it){
        Handle<String> script=ReadScript(getIsolate(), *it);
        if (!script.IsEmpty()){
            Local<Script> compiledScript=compileScript(script);
            runScript(compiledScript);
        }
    }
}

void JavaScriptVM::finalize(){
    isolate->Exit();
    delete messageFactory;
}

}
