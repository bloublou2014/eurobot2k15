#ifndef _TASKMANAGER_H
#define _TASKMANAGER_H

#include <boost/heap/fibonacci_heap.hpp>
#include <map>
#include <string>
#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>

#include "core/Node.h"
#include "tasks/AbstractTask.h"
#include "messages/Message.h"
#include "core/AbstractMessageHandler.h"

using boost::heap::fibonacci_heap;
using std::map;
using std::string;
using std::queue;
using boost::mutex;
using boost::condition_variable;
using boost::unique_lock;
using boost::lock_guard;
using std::pair;

namespace robot{

//Every task has its priority
struct RankedTask{
    AbstractTask* task;
    int rank;
};

//Used in priority queue for comparing task priority
struct compareRankedTask{
    bool operator()(const RankedTask& t1, const RankedTask& t2)const{
        TaskState state1=t1.task->getTaskState();
        TaskState state2=t2.task->getTaskState();
        if (state1!=state2){
            return state1>state2;
        }
        else{
            return t1.rank>t2.rank;
        }
    }
};

typedef fibonacci_heap<RankedTask, boost::heap::compare<compareRankedTask>> TaskQueue;

class TaskManager: public Node, public AbstractMessageHandler{
public:
    TaskManager():Node("TaskManager"),shouldStop(false){}

    //task calls this to update its state
    void updateStatus(AbstractTask* taskSender, TaskState newState);
    //TODO: must implement world class first
    bool getWorldProperty() const;
    bool setWorldProperty();
    //Executor manager will send messages to tasks using this method
    bool sendMessage(Message* message);
    //Executor manager will receive messages from tasks and send pass them to executor manager
    bool receiveMessage(Message* message);

    //Loading tasks
    bool addTask(AbstractTask* newTask);

    //caled before thread is created to initialize values, or load configuration
    void init();
    //called to stop
    void stop();

    void setExecutorManager(AbstractMessageHandler* _executorManager);
protected:
    //thread task
    void main();
    void startAllTasks();
    //when new message is received in queue it needs to be forwarded to tasks
    void dispatchMessage();
private:
    typedef pair<RankedTask,TaskQueue::handle_type> CachedRankedTask;
    mutex heapModification;
    map<string,CachedRankedTask> taskCache;
    TaskQueue orderedTasks;

    Message* popNextMessage();
    mutex messageQueueLock;
    condition_variable messageQueueNotEmpty;
    queue<Message*> messageQueue;

    bool shouldStop;
    AbstractMessageHandler* executorManager;
};

}


#endif //_TASKMANAGER_H
