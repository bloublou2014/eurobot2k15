#include "EnemyDetectorCommand.h"

namespace motion{

string AddStaticObject::NAME="AddStaticObject";
void AddStaticObject::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, AddStaticObject::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(AddStaticObject::NAME, tpl->GetFunction());
    provider->setObjectTemplate(AddStaticObject::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, AddStaticObject::NAME.c_str()), tpl->GetFunction());
}

void AddStaticObject::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        vector<Point2D> result;
        if (args[0]->IsArray()){
            Handle<Array> arr=Handle<Array>::Cast(args[0]);
            int length = arr->Length();
            for(int i = 0; i < length; i++)
            {
                v8::Local<v8::Object> element = arr->Get(i);
                int x=element->Get(String::NewFromUtf8(isolate,"x"))->ToObject()->Uint32Value();
                int y=element->Get(String::NewFromUtf8(isolate,"y"))->ToObject()->Uint32Value();
                cout<<"Adding point: "<<x<<", "<<y<<endl;
                result.push_back(Point2D(x,y));
            }
            AddStaticObject* obj = new AddStaticObject(result);
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        }else{
            isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Params must be array of Point objects"));
        }
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

}
