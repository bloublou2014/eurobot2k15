#ifndef _EXECUTORMANAGER_H
#define _EXECUTORMANAGER_H

#include <map>
#include <string>
#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>

#include "core/Node.h"
#include "executors/AbstractExecutor.h"
#include "messages/Message.h"
#include "core/AbstractMessageHandler.h"
#include "messages/StopMessage.h"

using std::map;
using std::string;
using std::queue;
using boost::mutex;
using boost::shared_mutex;
using boost::condition_variable;
using boost::unique_lock;
using boost::shared_lock;
using boost::upgrade_lock;
using boost::lock_guard;

namespace robot{

class ExecutorManager:public Node, public AbstractMessageHandler{
public:
    ExecutorManager():Node("ExecutorManager"),shouldStop(false){}

    bool getWorldProperty() const;
    bool setWorldProperty();

    //Executor manager will send messages to tasks using this method
    bool sendMessage(Message* message);
    //Executor manager will receive messages from tasks and send pass them to executor manager
    bool receiveMessage(Message* message);

    bool addExecutor(AbstractExecutor* newExecutor);

    void init();
    void stop();

    void setTaskManager(AbstractMessageHandler* _taskManager);
protected:

    void main();
    void startAllExecutors();
    void stopAllExecutors();
    void dispatcheMessage();
private:
    shared_mutex executorsMapManipulation;
    map<string,AbstractExecutor*> executorsMap;

    Message* popNextMessage();
    mutex messageQueueLock;
    condition_variable messageQueueNotEmpty;
    queue<Message*> messageQueue;

    bool shouldStop;
    AbstractMessageHandler* taskManager;
};

}

#endif //_EXECUTORMANAGER_H
