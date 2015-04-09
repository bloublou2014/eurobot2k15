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
    AbstractTask* task;
    int rank;
    int estimatedRunningTime;
};

class TaskManager: public Node, public TaskManagerInterface{
public:
    static bool larger(const RankedTask& t1, const RankedTask& t2);

    TaskManager(const string& strategy, const string &directory);

    //task calls this to update its state
    bool updateStatus(const string& taskName, TaskState newState);
    //TODO: must implement world class first
    bool getWorldProperty() const;
    bool setWorldProperty();
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
protected:
    //thread task
    void main();
    void startAllTasks();
    void stopAllTasks();
    //when new message is received in queue it needs to be forwarded to tasks
    void dispatchMessage();
private:
    void createTask(const string& name, const string& filename, int rank, int duration, const string &directory);

//    typedef pair<RankedTask,TaskQueue::handle_type> CachedRankedTask;
    mutex tasksLock;
    map<string, RankedTask> availableTasks;
    AbstractTask* currentlyRunningTask;
    Message* popNextMessage();
    mutex messageQueueLock;
    condition_variable messageQueueNotEmpty;
    queue<Message*> messageQueue;

    bool shouldStop;
    bool matchStarted;
    AbstractMessageHandler* executorManager;
};

}


#endif //_TASKMANAGER_H
