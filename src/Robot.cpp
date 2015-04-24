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
    JumpersExecutor jumpExec;
    EnemyDetector enemyDetExec;
    MotionExecutor motionExec;
    LiftLeftExecutor liftLeftExec;
    LiftCenterExecutor liftCenterExec;
    LiftRightExecutor liftRightExec;
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
    execMgr->addExecutor(&jumpExec);
    execMgr->addExecutor(&enemyDetExec);
    execMgr->addExecutor(&motionExec);
    execMgr->addExecutor(&liftLeftExec);
    execMgr->addExecutor(&liftCenterExec);
    execMgr->addExecutor(&liftRightExec);
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

//    boost::this_thread::sleep(boost::posix_time::milliseconds(1500));
//getchar();
//        execMgr->receiveMessage(new StartMessage(StartMessage::Color::YELLOW, "Milan"));

    taskMgr->join();
    execMgr->join();

    return 0;
}





//#include <iostream>
//#include "utils/pathFinding/PathFinding.h"

//using namespace std;
//using namespace path_finding;
//using namespace geometry;

//void test_visual(PathFinding &pf);
//void test(PathFinding &pf);

//string OBSTACLE = "OBSTACLE";
//string POINT = "POINT";

//int main()
//{
//    cout << "Hello world!" << endl;

//    PathFinding pf(1000, -1000, 1000, -1000);

//    test(pf);

////    test_visual(pf);

//    return 0;
//}

//void test_visual(PathFinding &pf)
//{
//    int startX, startY, goalX, goalY;
//    char input[255];
//    vector<int> obstacleIds;

//    while(1)
//    {
//        // read data
//        cin >> startX;
//        cin >> startY;
//        cin >> goalX;
//        cin >> goalY;

//        vector<Point2D> removedObstaclePoints;
//        removedObstaclePoints.push_back(Point2D(400,200));
//        removedObstaclePoints.push_back(Point2D(200 + 370, 200 + 29));
//        removedObstaclePoints.push_back(Point2D(200 + 370, 400 + 29));
//        removedObstaclePoints.push_back(Point2D(400,400));

//        int removedId = pf.addObstacle(removedObstaclePoints);

//        while(1)
//        {
//            cin >> input;
//            if(OBSTACLE.compare(input)==0)
//            {
//                vector<Point2D> obstaclePoints;

//                // add all points
//                while(1)
//                {
//                    cin >> input;
//                    if(POINT.compare(input)==0)
//                    {
//                        int x,y;
//                        cin >> x;
//                        cin >> y;

//                        Point2D point(x,y);
//                        obstaclePoints.push_back(point);
//                    }
//                    else // END_OBSTACLE
//                    {
//                        break;
//                    }
//                }

//                // add obstacle
//                int id = pf.addObstacle(obstaclePoints);
//                obstacleIds.push_back(id);
//            }
//            else
//            {
//                break;
//            }
//        }

//        pf.removeObstacle(removedId);

//        Point2D start(startX,startY);
//        Point2D goal(goalX,goalY);
//        deque<Point2D> path;

//        if(pf.search(start, goal, path))
//        {
//            for(deque<Point2D>::iterator pointIter = path.begin(); pointIter != path.end(); pointIter++)
//            {
//                int x = pointIter->getX();
//                int y = pointIter->getY();

//                cout << "x" << endl << x << endl << "y" << endl << y << endl;
//            }
//        }

//        cout << "end" << endl;

//        for(vector<int>::iterator idIter = obstacleIds.begin(); idIter != obstacleIds.end(); idIter++)
//        {
//            try
//            {
//                pf.removeObstacle(*idIter);
//            }
//            catch(...){}
//        }

//        obstacleIds.clear();
//    }
//}

//void test(PathFinding &pf)
//{
//    int id;

//    Point2D start(0,0);
//    Point2D goal(100,100);

//    deque<Point2D> path;

//    vector<Point2D> obstaclePoints;
//    obstaclePoints.push_back(Point2D(0,0));
//    obstaclePoints.push_back(Point2D(0,10));
//    obstaclePoints.push_back(Point2D(10,10));
//    obstaclePoints.push_back(Point2D(10,0));
//    id = pf.addObstacle(obstaclePoints);

//    //pf.removeObstacle(id);

//    cout<<"obstacles count: "<<pf.getAllObstacles().size()<<endl;

//    if(pf.search(start, goal, path))
//    {
//        cout << "Found path:"<< endl;

//        for(deque<Point2D>::iterator pointIter = path.begin(); pointIter != path.end(); pointIter++)
//        {
//            int x = pointIter->getX();
//            int y = pointIter->getY();

//            cout << "	point: X: "<<x<<", Y: "<<y<<endl;
//        }
//    }
//    else
//    {
//        cout << "Can't find path."<<endl;
//    }
//}
