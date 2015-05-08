#ifndef _TASKMANAGER_H
#define _TASKMANAGER_H

#include <boost/heap/fibonacci_heap.hpp>
#include <map>
#include <string>
#include <queue>
#include <algorithm>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "core/Node.h"
#include "tasks/AbstractTask.h"
#include "tasks/JavaScriptTask.h"
#include "messages/Message.h"
#include "core/AbstractMessageHandler.h"
#include "messages/StopMessage.h"
#include "core/TaskManagerInterface.h"
#include "executors/msg/TimePassed.h"
#include "core/TaskStateChangeNotification.h"

using boost::heap::fibonacci_heap;
using std::map;
using std::string;
using std::queue;
using boost::mutex;
using boost::condition_variable;
using boost::unique_lock;
using boost::lock_guard;
using std::pair;
using std::max_element;

namespace robot{

//Every task has its priority
struct RankedTask{
    RankedTask():task(NULL),rank(0),estimatedRunningTime(0), finalize(false){}

    AbstractTask* task;
    int rank;
    int estimatedRunningTime;
    bool finalize;
};

class TaskManager: public Node, public TaskManagerInterface{
public:
    static bool larger(const RankedTask& t1, const RankedTask& t2);

    TaskManager(const string& strategy, const string &directory);

    //task calls this to update its state
    bool updateStatus(const string& taskName, TaskState newState);
    //TODO: must implement world class first
    string getWorldProperty(const string& key);
    void setWorldProperty(const string& key, const string& value);
    //Executor manager will send messages to tasks using this method
    bool sendMessage(Message* message);
    //Executor manager will receive messages from tasks and send pass them to executor manager
    bool receiveMessage(Message* message);

    //Loading tasks
    bool addTask(RankedTask &rankedTask);

    //caled before thread is created to initialize values, or load configuration
    void init();
    //called to stop
    void stop();

    void runBestTask();

    void setExecutorManager(AbstractMessageHandler* _executorManager);

    StartMessage::Color getMatchColor();
    bool isMatchStarted() const;
protected:
    //thread task
    void main();
    void startAllTasks();
    void stopAllTasks();
    //when new message is received in queue it needs to be forwarded to tasks
    void dispatchMessage();
private:
    void createTask(const string& name, int rank, const string &directory, bool finalize);

//    typedef pair<RankedTask,TaskQueue::handle_type> CachedRankedTask;
    mutex tasksLock;
    map<string, RankedTask> availableTasks;
    AbstractTask* currentlyRunningTask;
    RankedTask finalizeTask;
    void checkTime(TimePassedNotification* tp);
    void finalizeMatch();

    Message* popNextMessage();
    mutex messageQueueLock;
    condition_variable messageQueueNotEmpty;
    queue<Message*> messageQueue;

    bool shouldStop;
    bool matchStarted;
    bool matchFinished;
    AbstractMessageHandler* executorManager;

    static const int matchDuration;
    StartMessage::Color matchColor;

    mutex worldStateLock;
    map<string,string> worldState;
};

}


#endif //_TASKMANAGER_H
