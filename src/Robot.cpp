#include <iostream>
#include <csignal>

#include "core/TaskManager.h"
#include "core/ExecutorManager.h"

#include "tasks/TestTask.h"
#include "tasks/ServoTask.h"
#include "tasks/PipeTask.h"

#include "executors/ExampleExecutor.h"
#include "executors/MotionExecutor.h"
#include "executors/servo/ServoTest/ServosExecutor.h"

#include "executors/liftcenter/LiftCenterExecutor.h"
#include "executors/liftleft/LiftLeftExecutor.h"
#include "executors/liftright/LiftRightExecutor.h"
#include "executors/popcorn/PopcornExecutor.h"
#include "executors/flap/FlapExecutor.h"

#include "utils/helper/LiftState.h" // tome test izbaciti kasnije

#define CROSS_COMPILING

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

    //TestTask t1("task1");     // MOTION  TASK
    //TestTask t2("task2");     // ne radi nista :D
    //ExampleExecutor e1;         // SERVO  TASK
    //ServoTask t3("ServoTask");   // SERVO TASK
    PipeTask pipeTask("PipeTask"); // PIPE TASK


#ifdef CROSS_COMPILING
    MotionExecutor motionExec;    // MOTION EXECUTOR
   // ServoExecutor servoExec;  // SERVO EXECUTOR

    LiftCenterExecutor liftCenterExec;
    LiftRightExecutor liftRightExec;
    LiftLeftExecutor liftLeftExec;
    PopcornExecutor popcornExec;
    FlapExecutor flapExec;

#endif
    taskMgr.setExecutorManager(&execMgr);
    execMgr.setTaskManager(&taskMgr);

    //taskMgr.addTask(&t1);           // MOTION TASK
    //execMgr.addExecutor(&e1);

    //taskMgr.addTask(&t3);      // SERVO TASK
    taskMgr.addTask(&pipeTask);  // PIPE TASK
#ifdef CROSS_COMPILING
    execMgr.addExecutor(&motionExec);  // MOTION EXECUTOR
    //execMgr.addExecutor(&servoExec);
    execMgr.addExecutor(&liftCenterExec);
    execMgr.addExecutor(&liftRightExec);
    execMgr.addExecutor(&liftLeftExec);
    execMgr.addExecutor(&popcornExec);
    execMgr.addExecutor(&flapExec);

#endif

    taskMgr.init();
    execMgr.init();

    taskMgr.start();
    execMgr.start();

    cout<<"Everything is started"<<endl;

    //while(true){
        /*
        string topic;
        string sender;
        cout<<"Enter topic name: "<<endl;
        cin>>topic;
        cout<<"Enter sender name: "<<endl;
        cin>>sender;
        */
        for ( int i = 0; i < 1 ; i++ ){
//            Notification* m = new Notification("liftNotification","asd");
//            taskMgr.sendMessage(m);
            Notification* n=new Notification("milan","asd");
            taskMgr.sendMessage(n);
            LiftState state;


        }
      //  break;
    //}

    //    Notification* n=new Notification("milan","jova");
    //    taskMgr.sendMessage(n);

    taskMgr.join();
    execMgr.join();

    return 0;
}
