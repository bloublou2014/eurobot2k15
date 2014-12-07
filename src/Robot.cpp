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



    while(true){
        string topic;
        string sender;
        cout<<"Enter topic name: "<<endl;
        cin>>topic;
        cout<<"Enter sender name: "<<endl;
        cin>>sender;
        Notification* n=new Notification(topic,sender);
        taskMgr.receiveMessage(n);
    }
    taskMgr.join();

    return 0;
}
