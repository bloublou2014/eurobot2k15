#include <iostream>

#include "core/TaskManager.h"
#include "tasks/TestTask.h"

using namespace std;
using namespace robot;

int main(int argn, char** argc){
    TestTask t1("task1");
    TestTask t2("task2");
    TestTask t3("task3");

    TaskManager taskMgr;

    taskMgr.init();
    taskMgr.addTask(&t1);
    taskMgr.addTask(&t2);
    taskMgr.addTask(&t3);
    taskMgr.start();


    cout<<"Waiting to finish"<<endl;

    Notification n1("testNotification","Milan");

    taskMgr.receiveMessage(&n1);

    taskMgr.join();

    return 0;
}
