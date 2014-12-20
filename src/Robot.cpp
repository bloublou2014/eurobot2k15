#include <iostream>
#include <csignal>

#include "core/TaskManager.h"
#include "core/ExecutorManager.h"
#include "tasks/TestTask.h"
#include "executors/ExampleExecutor.h"
#include "executors/MotionExecutor.h"

using namespace std;
using namespace robot;
using namespace motion;

TaskManager taskMgr;
ExecutorManager execMgr;

void signalHandler(int sigNum){
    cout<<"Stopping robot execution!"<<endl;

    taskMgr.stop();
    execMgr.stop();
}

int main(int argn, char** argc){
    signal(SIGINT,signalHandler);

    TestTask t1("task1");
    TestTask t2("task2");
    ExampleExecutor e1;
    MotionExecutor motionExec;

    taskMgr.setExecutorManager(&execMgr);
    execMgr.setTaskManager(&taskMgr);

    taskMgr.addTask(&t1);
    execMgr.addExecutor(&e1);
    execMgr.addExecutor(&motionExec);

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
