#include <iostream>
#include "executors/ExampleExecutor.h"

using namespace std;
using namespace robot;

int main(int argn, char** argc){
    ExampleExecutor ex;
    ex.init();

    Notification n("testEvent","milan");

    if (ex.processNotification(&n))
        cout<<"success"<<endl;
    else
        cout<<"fail"<<endl;

    return 0;
}
