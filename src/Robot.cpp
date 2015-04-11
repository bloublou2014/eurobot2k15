#include <iostream>
#include <csignal>

#include "core/TaskManager.h"
#include "messages/StartMatchMessage.h"
#include "core/ExecutorManager.h"
#include "tasks/TestTask.h"
#include "tasks/JavaScriptTask.h"
#include "executors/ExampleExecutor.h"
#include "executors/MotionExecutor.h"

using namespace std;
using namespace robot;
using namespace motion;

TaskManager* taskMgr;
ExecutorManager* execMgr;

void signalHandler(int sigNum){
    cout<<"Stopping robot execution!"<<endl;

    taskMgr->stop();
    execMgr->stop();
}

int main(int argn, char** argc){

    if (argn<3){
        std::cout<<"Wrong run parameters"<<std::endl;
        return -1;
    }
    string strategy(argc[1]);
    string directory(argc[2]);

    signal(SIGINT,signalHandler);

    JavaScriptTask::InitV8Platform();

    taskMgr=new TaskManager(strategy, directory);
    execMgr=new ExecutorManager();

    ExampleExecutor e1;

#ifdef CROSS_COMPILING
    MotionExecutor motionExec;
#endif

    taskMgr->setExecutorManager(execMgr);
    execMgr->setTaskManager(taskMgr);

    execMgr->addExecutor(&e1);

#ifdef CROSS_COMPILING
    execMgr->addExecutor(&motionExec);
#endif

    taskMgr->init();
    execMgr->init();

    taskMgr->start();
    execMgr->start();

    getchar();
    taskMgr->sendMessage(new StartMessage("Milan"));

    taskMgr->join();
    execMgr->join();

    return 0;
}
