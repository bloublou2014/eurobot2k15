#include <iostream>

#include "core/TaskManager.h"
#include "core/ExecutorManager.h"
#include "tasks/TestTask.h"
#include "executors/ExampleExecutor.h"

using namespace std;
using namespace robot;

int main(int argn, char** argc){
    TestTask t1("task1");
    TestTask t2("task2");
    ExampleExecutor e1;

//    TestTask t3("task3");

    TaskManager taskMgr;
    ExecutorManager execMgr;

    taskMgr.setExecutorManager(&execMgr);
    execMgr.setTaskManager(&taskMgr);

    taskMgr.addTask(&t1);
    taskMgr.addTask(&t2);
    execMgr.addExecutor(&e1);

    taskMgr.init();
    execMgr.init();

    taskMgr.start();
    execMgr.start();

    cout<<"Everything is started"<<endl;

    while(true){
        string topic;
        string sender;
        cout<<"Enter topic name: "<<endl;
        cin>>topic;
        cout<<"Enter sender name: "<<endl;
        cin>>sender;
        Notification* n=new Notification(topic,sender);
        taskMgr.sendMessage(n);
    }

//    Notification* n=new Notification("milan","jova");
//    taskMgr.sendMessage(n);

    taskMgr.join();
    execMgr.join();

    return 0;
}
