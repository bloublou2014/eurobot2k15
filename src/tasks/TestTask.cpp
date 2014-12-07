#include "TestTask.h"

namespace robot {

void TestTask::init(){

}

void TestTask::initScript(){
    this->subscribe("testNotification",(notificationCallback)&TestTask::handleTestNotifications);
    this->subscribe("milan",(notificationCallback)&TestTask::handleMilanNotifications);
}

void TestTask::startScript(){
    debug("Script is started");
}

void TestTask::stopScript(){

}

void TestTask::handleTestNotifications(Notification* testNotification){
    debug("Hello, this notification has been handled. Received from: "+testNotification->getSource());
}

void TestTask::handleMilanNotifications(Notification* testNotification){
    debug("Milan notification received from: "+testNotification->getSource());
}

}
