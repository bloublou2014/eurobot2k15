#include "ExampleExecutor.h"

#include <iostream>

using namespace std;

namespace robot {

string ExampleExecutor::NAME="ExampleNode";

void ExampleExecutor::init(){
    this->subscribe("testEvent",(notificationCallback)&ExampleExecutor::processTestEvent);
}

void  ExampleExecutor::processTestEvent(Notification* notification){
     cout<<"Received notification! Yea! from: "<<notification->getSource()<<endl;
}

}
