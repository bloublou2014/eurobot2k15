#include <iostream>
#include "executors/ExampleExecutor.h"

using namespace std;
using namespace robot;

int main(int argn, char** argc){
    ExampleExecutor ex;
    ex.init();
    ex.start();

    Notification n("testEvent","milan");

    if (ex.processNotification(&n))
        cout<<"success"<<endl;
    else
        cout<<"fail"<<endl;

    ex.join();

    return 0;
}
