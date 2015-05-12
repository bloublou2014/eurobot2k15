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
#include "executors/jumpers/jumpers.h"

#include "executors/carpet/CarpetExecutor.h"

#include "tasks/PipeTask.h"
#include "messages/StartMatchMessage.h"
#define CROSS_COMPILING
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

    if (argn<4){
        std::cout<<"Wrong run parameters"<<std::endl;
        return -1;
    }
    string strategy(argc[1]);
    string scheduler(argc[2]);
    string directory(argc[3]);

    signal(SIGINT,signalHandler);

    JavaScriptTask::InitV8Platform();

    taskMgr=new TaskManager(strategy, scheduler, directory);
    execMgr=new ExecutorManager();

    ExampleExecutor e1;

    //PipeTask testTask("pipeTask");
    //taskMgr->addTask(&testTask);


#ifdef CROSS_COMPILING

#ifdef VELIKI_ROBOT
    EnemyDetector enemyDetExec;
    MotionExecutor motionExec;
    LiftLeftExecutor liftLeftExec;
    LiftCenterExecutor liftCenterExec;
    LiftRightExecutor liftRightExec;
    PopcornExecutor popcornExec;
    FlapExecutor flapExec;
    JumpersExecutor jumpExec;
#endif

#ifdef MALI_ROBOT
    JumpersExecutor jumpExec;
    EnemyDetector enemyDetExec;
     MotionExecutor motionExec;
    CarpetExecutor carpetExec;

#endif

#endif

    taskMgr->setExecutorManager(execMgr);
    execMgr->setTaskManager(taskMgr);

    execMgr->addExecutor(&e1);

#ifdef CROSS_COMPILING

#ifdef VELIKI_ROBOT
    execMgr->addExecutor(&enemyDetExec);
    execMgr->addExecutor(&motionExec);
    execMgr->addExecutor(&liftLeftExec);
    execMgr->addExecutor(&liftCenterExec);
    execMgr->addExecutor(&liftRightExec);
    execMgr->addExecutor(&popcornExec);
    execMgr->addExecutor(&flapExec);
    execMgr->addExecutor(&jumpExec);
#endif

#ifdef MALI_ROBOT
    execMgr->addExecutor(&jumpExec);
    execMgr->addExecutor(&carpetExec);
    execMgr->addExecutor(&enemyDetExec);
    execMgr->addExecutor(&motionExec);

#endif

#endif
//    StartMessage::Color collor;
//    collor = StartMessage::Color::YELLOW;

    taskMgr->init();
    execMgr->init();

    taskMgr->start();
    execMgr->start();

    getchar();
//    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    execMgr->receiveMessage(new StartMessage(StartMessage::Color::YELLOW, "Milan"));

    taskMgr->join();
    execMgr->join();

    return 0;
}

