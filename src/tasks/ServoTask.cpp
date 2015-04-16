#include  "ServoTask.h"


namespace robot{

void ServoTask::init(){

}

void ServoTask::initScript(){
//    this->subscribe(LiftNotification::notificationNAME,(notificationCallback)&ServoTask::setState);
    this->subscribe("milan",(notificationCallback)&ServoTask::handleMilanNotifications);

    state.Aveable = true;
}

void ServoTask::startScript(){
    debug("Servo is started");

}

void ServoTask::stopScript(){
    shouldStop = true;
    cout << "killing Servo task, SholudStop = " << shouldStop ;

}

void ServoTask::handleMilanNotifications(Notification* testNotification){
    debug("Milan notification received");
    while(!shouldStop){

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        GetLiftState* getCmd = new GetLiftState(LiftRightExecutor::NAME);
        sendCommand(getCmd, (responseCallback)&ServoTask::liftStateRecive, (responseCallback)&ServoTask::servoError);
        //debug("respining Servo Taks");

        //UnloadPopcorn* cmd12 =(UnloadPopcorn*) ActuatorAction::Flap(ActuatorCommandType::ActuatorType::KICK);
        //sendCommand(cmd12 ,(responseCallback)&ServoTask::rotateFor, (responseCallback)&ServoTask::error );
        if(state.Aveable){

            /*
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        RotateFor* cmd = new RotateFor(250);
        sendCommand(cmd, (responseCallback)&ServoTask::rotateFor, (responseCallback)&ServoTask::servoError);
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        RotateToPosition* cmd_2 = new RotateToPosition(200);
        sendCommand(cmd_2, (responseCallback)&ServoTask::roteteToPosition, (responseCallback)&ServoTask::servoError);

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        RotateFor* cmd_3 = new RotateFor(300);
        sendCommand(cmd_3, (responseCallback)&ServoTask::rotateFor, (responseCallback)&ServoTask::servoError);
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        RotateToPosition* cmd_4 = new RotateToPosition(400);
        sendCommand(cmd_4, (responseCallback)&ServoTask::roteteToPosition, (responseCallback)&ServoTask::servoError);
        */



            //GetObject* cmd5 = new GetObject(ActuatorCommand::Executors::LIFT_LEFT);
            //            sendCommand(cmd5, (responseCallback)&ServoTask::rotateFor, (responseCallback)&ServoTask::error);
        }
        // boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        //GetObject* cmd5 = Actuators::LiftCenter(GET_OBJECT);


        if (state.Aveable){
            GetObject* cmd6 = (GetObject*) ActuatorAction::LiftRight(ActuatorType::GET_OBJECT);
            sendCommand(cmd6 ,(responseCallback)&ServoTask::rotateFor, (responseCallback)&ServoTask::error );

            GetObject* cmd10 = (GetObject*) ActuatorAction::LiftRight(ActuatorType::RELOAD_CONFIG);
            sendCommand(cmd10, (responseCallback)&ServoTask::rotateFor, (responseCallback)&ServoTask::error);
            state.Aveable = false;
        }



        if(state.Aveable){
            GetObject* cmd7 = (GetObject*) ActuatorAction::LiftRight(ActuatorType::UNLOAD_OBJECT);
            sendCommand(cmd7 ,(responseCallback)&ServoTask::rotateFor, (responseCallback)&ServoTask::error );
        }

        if(state.Aveable){
            GetObject* cmd8 = (GetObject*) ActuatorAction::LiftRight(ActuatorType::GET_OBJECT);
            sendCommand(cmd8 ,(responseCallback)&ServoTask::rotateFor, (responseCallback)&ServoTask::error );
        }

        if(state.Aveable){
            GetObject* cmd9 = (GetObject*) ActuatorAction::LiftRight(ActuatorType::UNLOAD_OBJECT);
            sendCommand(cmd9, (responseCallback)&ServoTask::rotateFor, (responseCallback)&ServoTask::error );
        }
    }
}

void ServoTask::servoError(CommandResponse* resp){
    debug("Eror in servo");


}

void ServoTask::rotateFor(CommandResponse* resp){
    debug("Successs");


}

void ServoTask::roteteToPosition(CommandResponse* resp){
    debug("Successs");

}

void ServoTask::setState(Notification *resp){
//    debug("LiftNotication recived");
//    LiftNotification * stateResponse=(LiftNotification*) resp;
//    state = stateResponse->getState();

}
void ServoTask::liftStateRecive(CommandResponse* resp){
    debug("Response");
    GetLiftStateResponse* stateResponse = (GetLiftStateResponse*) resp;
    state = stateResponse->getState();
}

}
