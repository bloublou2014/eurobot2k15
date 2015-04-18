#include <iostream>
#include <csignal>

#include "core/TaskManager.h"
#include "messages/StartMatchMessage.h"
#include "core/ExecutorManager.h"
#include "tasks/TestTask.h"
#include "tasks/JavaScriptTask.h"
#include "executors/ExampleExecutor.h"
#include "executors/MotionExecutor.h"

#include "executors/liftcenter/LiftCenterExecutor.h"
#include "executors/liftleft/LiftLeftExecutor.h"
#include "executors/liftright/LiftRightExecutor.h"
#include "executors/popcorn/PopcornExecutor.h"
#include "executors/flap/FlapExecutor.h"
#include "executors/enemyDetector/EnemyDetector.h"

#include "executors/carpet/CarpetExecutor.h"

#include "tasks/PipeTask.h"

//#define CROSS_COMPILING
#define VELIKI
//#define MALI

using namespace std;
using namespace robot;
using namespace motion;
using namespace executor;

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

    //PipeTask testTask("pipeTask");
    //taskMgr->addTask(&testTask);


#ifdef CROSS_COMPILING

#ifdef VELIKI
    EnemyDetector enemyDetExec;
    MotionExecutor motionExec;
    LiftCenterExecutor liftCenterExec;
    LiftRightExecutor liftRightExec;
    LiftLeftExecutor liftLeftExec;
    PopcornExecutor popcornExec;
    FlapExecutor flapExec;
#endif

#ifdef MALI
    CarpetExecutor carpetExec;
#endif

#endif

    taskMgr->setExecutorManager(execMgr);
    execMgr->setTaskManager(taskMgr);

    execMgr->addExecutor(&e1);

#ifdef CROSS_COMPILING

#ifdef VELIKI
    execMgr->addExecutor(&enemyDetExec);
    execMgr->addExecutor(&motionExec);
    execMgr->addExecutor(&liftCenterExec);
    execMgr->addExecutor(&liftRightExec);
    execMgr->addExecutor(&liftLeftExec);
    execMgr->addExecutor(&popcornExec);
    execMgr->addExecutor(&flapExec);
#endif

#ifdef MALI
    execMgr->addExecutor(&carpetExec);
#endif

#endif

    taskMgr->init();
    execMgr->init();

    taskMgr->start();
    execMgr->start();

    getchar();
//    boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    taskMgr->sendMessage(new StartMessage("Milan"));

    taskMgr->join();
    execMgr->join();

    return 0;
}
