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

struct RankedTask{
    AbstractTask* task;
    int rank;
    //fibonacci_heap<RankedTask, boost::heap::compare<compareRankedTask>>::handle_type handle;
};

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

class TaskManager: public Node{
public:
    TaskManager():Node("TaskManager"),shouldStop(false){}

    void updateStatus(AbstractTask* task, TaskState newState);
    //TODO: must implement world class first
    bool getWorldProperty() const;
    bool setWorldProperty();
    bool receiveMessage(Message* message);

    bool addTask(AbstractTask* newTask);

    void init();
    void stop();
protected:
    void main();
    void startAllTasks();
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
};

}


#endif //_TASKMANAGER_H
