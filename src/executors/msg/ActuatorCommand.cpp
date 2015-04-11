#include "ActuatorCommand.h"

namespace executor {

ExecutorsMap ExecutorName={
    {LIFT_CENTER,"LiftCenterExecutor"},
    {LIFT_LEFT,"LiftLeftExecutor"},
    {LIFT_RIGHT,"LiftRightExecutor"},
    {POPCORN,"PopcornExecutor"},
    {FLAP,"FlapExecutor"}
};

string ActuatorCommand::NAME="ActuatorCommand";

Command* ActuatorAction::LiftLeft(ActuatorType _type){
    switch (_type){
    case GET_OBJECT :    return (Command*) new GetObject(LIFT_LEFT);
    case UNLOAD_OBJECT : return (Command*) new UnloadObject(LIFT_LEFT);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(LIFT_LEFT);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(LIFT_LEFT);
    default : return NULL;
    }
}

Command* ActuatorAction::LiftRight(ActuatorType _type){
    switch (_type){
    case GET_OBJECT :    return (Command*) new GetObject(LIFT_RIGHT);
    case UNLOAD_OBJECT : return (Command*) new UnloadObject(LIFT_RIGHT);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(LIFT_RIGHT);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(LIFT_RIGHT);
    default : return NULL;
    }
}

Command* ActuatorAction::LiftCenter(ActuatorType _type){
    switch (_type){
    case GET_OBJECT :    return (Command*) new GetObject(LIFT_CENTER);
    case UNLOAD_OBJECT : return (Command*) new UnloadObject(LIFT_CENTER);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(LIFT_CENTER);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(LIFT_CENTER);
    default : return NULL;
    }
}

Command* ActuatorAction::Flap(ActuatorType _type){
    switch (_type){
    case KICK_RIGHT :    return (Command*) new KickRight(FLAP);
    case KICK_LEFT :   return (Command*) new KickLeft(FLAP);
    case UNKICK_RIGHT : return (Command*) new UnKickRight(FLAP);
    case UNKICK_LEFT  :  return (Command*) new UnKickLeft(FLAP);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(FLAP);
    default : return NULL;
    }
}
Command* ActuatorAction::Popcorn(ActuatorType _type){
    switch (_type){
    case UNLOAD_POPCORN :    return (Command*) new UnloadPopcorn(POPCORN);
    case GET_POPCORN : return (Command*) new GetPopcorn(POPCORN);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(POPCORN);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(POPCORN);
    default : return NULL;
    }
}

Command* ActuatorConfig::LiftLeft(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(LIFT_LEFT, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(LIFT_LEFT, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::LiftRight(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(LIFT_RIGHT, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(LIFT_RIGHT, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::LiftCenter(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(LIFT_CENTER, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(LIFT_CENTER, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::Popcorn(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(POPCORN, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(POPCORN, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::Flap(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(FLAP, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(FLAP, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

string ActuatorCommandJS::NAME = "ActuatorCommand";

void ActuatorCommandJS::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, ActuatorCommandJS::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(ActuatorCommandJS::NAME, tpl->GetFunction());
    provider->setObjectTemplate(ActuatorCommandJS::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, ActuatorCommandJS::NAME.c_str()), tpl->GetFunction());
}

void ActuatorCommandJS::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    bool success;

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`

        v8::String::Utf8Value executorV8string ( args[0]->ToString() ) ;
        v8::String::Utf8Value actionV8string ( args[1]->ToString() );

        string _executorString = string(*executorV8string);
        string _actionString = string(*actionV8string);

        Command* obj;

        obj = parseCreateCommand(_executorString, _actionString, &success);
        std::cout<<"returned from parseCreateCommand" << std::endl;
        if(success){
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        }else{
            isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong parameters, wrong executor or aciton"));
        }
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

Command* ActuatorCommandJS::parseCreateCommand(string _executorString, string _actionString, bool* _success){
    Executors executorTmp;
    ActuatorType actionTmp;
    Command* tmp;
    *_success = true;

    if (_executorString.compare("LiftLeft") == 0 ) executorTmp = LIFT_LEFT;
    else if(_executorString.compare("LiftLeft") == 0) executorTmp = LIFT_RIGHT;
    else if(_executorString.compare("LiftCenter") == 0) executorTmp = LIFT_CENTER;
    else if(_executorString.compare("Flap") == 0) executorTmp = FLAP;
    else if(_executorString.compare("Popcorn") == 0) executorTmp = POPCORN;
    else {
        std::cout << "ERROR SREDITI OVO" << std::endl;
        executorTmp = NULL_EXECUTOR;
        *_success = false;
    }

    if (executorTmp == LIFT_LEFT || executorTmp == LIFT_RIGHT || executorTmp == LIFT_CENTER) {
        if(_actionString.compare("GetObject") == 0) actionTmp = GET_OBJECT;
        else if(_actionString.compare("UnloadObject")) actionTmp = UNLOAD_OBJECT;
        else {
            actionTmp = NULL_ACTION;
            *_success = false;
        }
    }else if(executorTmp == POPCORN){
        if(_actionString.compare("GetPopcorn") == 0) actionTmp = GET_POPCORN;
        else if(_actionString.compare("UnloadPopcorn") == 0) actionTmp = UNLOAD_POPCORN;
        else {
            actionTmp = NULL_ACTION;
            *_success = false;
        }
    }else if(executorTmp == FLAP){
        if(_actionString.compare("KickLeft") == 0) actionTmp = KICK_LEFT;
        else if(_actionString.compare("KickRight") == 0) actionTmp = KICK_RIGHT;
        else if(_actionString.compare("UnkickRight") == 0) actionTmp  = UNKICK_RIGHT;
        else if(_actionString.compare("UnkickLeft") == 0) actionTmp = UNKICK_LEFT;
        else{
            actionTmp = NULL_ACTION;
            *_success = false;
        }
    }

    if(!*_success){
        return NULL;
    }else{
        std::cout << "enetered creating object " << std::endl;
        switch(executorTmp){
        case LIFT_LEFT:{ tmp = ActuatorAction::LiftLeft(actionTmp); break; }
        case LIFT_RIGHT:{ tmp = ActuatorAction::LiftRight(actionTmp); break; }
        case LIFT_CENTER:{ tmp = ActuatorAction::LiftCenter(actionTmp); break; }
        case FLAP: { tmp = ActuatorAction::Flap(actionTmp); break; }
        case POPCORN: { std::cout << "TOME" << std::endl; tmp = ActuatorAction::Popcorn(actionTmp);   std::cout << "TOME" << std::endl; break; }
        case NULL_EXECUTOR: {break; }
        }
        std::cout << "TOME" << std::endl;
    }
    return tmp;

}

//void  parse(string _executorString, string _actuatorString, Executors *_executor, ActuatorType _action );

} // end namespace
