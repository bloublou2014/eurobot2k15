#include <iostream>
#include <csignal>

#include "core/TaskManager.h"
#include "core/ExecutorManager.h"
#include "tasks/TestTask.h"
#include "tasks/JavaScriptTask.h"
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

    JavaScriptTask::InitV8Platform();

    TestTask t1("task1");
    TestTask t2("task2");
    JavaScriptTask js1("js1","task1-1.js");
    JavaScriptTask js2("js2","task1.js");
    ExampleExecutor e1;

#ifdef CROSS_COMPILING
    MotionExecutor motionExec;
#endif

    taskMgr.setExecutorManager(&execMgr);
    execMgr.setTaskManager(&taskMgr);

    taskMgr.addTask(&t1);
    //taskMgr.addTask(&js1);
    taskMgr.addTask(&js2);
    execMgr.addExecutor(&e1);
#ifdef CROSS_COMPILING
    execMgr.addExecutor(&motionExec);
#endif

    taskMgr.init();
    execMgr.init();

    taskMgr.start();
    execMgr.start();

    cout<<"Everything is started"<<endl;

//    while(true){
//        string topic;
//        string sender;
//        cout<<"Enter topic name: "<<endl;
//        cin>>topic;
//        cout<<"Enter sender name: "<<endl;
//        cin>>sender;
//        Notification* n=new Notification(topic,sender);
//        taskMgr.sendMessage(n);
//    }

    taskMgr.join();
    execMgr.join();

    return 0;
}
