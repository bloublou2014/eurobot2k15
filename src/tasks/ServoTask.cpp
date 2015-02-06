#include  "ServoTask.h"


namespace robot{

void ServoTask::init(){

}

void ServoTask::initScript(){
    this->subscribe("milan",(notificationCallback)&ServoTask::handleMilanNotifications);
}

void ServoTask::startScript(){
    debug("Servo is started");
}

void ServoTask::stopScript(){

}

void ServoTask::handleMilanNotifications(Notification* testNotification){
    //debug("Milan notification received");
    if (this->counter < 5){
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

    }
}

void ServoTask::servoError(CommandResponse* resp){
    debug("Eror in servo");


}

void ServoTask::rotateFor(CommandResponse* resp){
    //debug("servo rotate");
    RotateFor * cmd = new RotateFor(200);
    sendCommand(cmd,(responseCallback)&ServoTask::rotateFor, (responseCallback)&ServoTask::servoError);

}

void ServoTask::roteteToPosition(CommandResponse* resp){
    //debug("servo rotate to position");
    RotateToPosition* cmd = new RotateToPosition(200);
    sendCommand(cmd, (responseCallback)&ServoTask::roteteToPosition, (responseCallback)&ServoTask::servoError);


}

}
