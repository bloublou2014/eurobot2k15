#include "TestTask.h"

namespace robot {

void TestTask::init(){

}

void TestTask::initScript(){
    this->subscribe("testNotification",(notificationCallback)&TestTask::handleTestNotifications);
}

void TestTask::startScript(){

}

void TestTask::stopScript(){

}

void TestTask::handleTestNotifications(Notification* testNotification){
    debug("Hello, this notification has been handled");
}

}
