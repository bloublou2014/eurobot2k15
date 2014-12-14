#include <iostream>
#include <csignal>

#include "core/TaskManager.h"
#include "core/ExecutorManager.h"
#include "tasks/TestTask.h"
#include "executors/ExampleExecutor.h"
#include "plugins/WebServer.h"

using namespace std;
using namespace robot;

TaskManager taskMgr;
ExecutorManager execMgr;
WebServer server(8888);

void signalHandler(int sigNum){
    cout<<"Stopping robot execution!"<<endl;

    taskMgr.stop();
    execMgr.stop();
    server.stop();
}

int main(int argn, char** argc){
    signal(SIGINT,signalHandler);

    TestTask t1("task1");
    TestTask t2("task2");
    ExampleExecutor e1;

    taskMgr.setExecutorManager(&execMgr);
    execMgr.setTaskManager(&taskMgr);

    taskMgr.addTask(&t1);
    taskMgr.addTask(&t2);
    execMgr.addExecutor(&e1);

    taskMgr.init();
    execMgr.init();

    taskMgr.start();
    execMgr.start();
    server.start();

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

//    Notification* n=new Notification("milan","jova");
//    taskMgr.sendMessage(n);

    //server.join();
    taskMgr.join();
    execMgr.join();
    server.join();

    return 0;
}
