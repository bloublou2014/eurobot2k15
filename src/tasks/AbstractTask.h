#ifndef _ABSTRACTTASK_H
#define _ABSTRACTTASK_H

#include <exception>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <queue>
#include <map>

#include "core/Node.h"
#include "core/CommandSource.h"
#include "core/NotificationHandler.h"
#include "core/NotificationSource.h"
#include "core/AbstractMessageHandler.h"
#include "core/TaskManagerInterface.h"
#include "core/JavaScriptVM.h"

using std::queue;
using std::map;
using boost::mutex;
using boost::condition_variable;
using boost::unique_lock;

using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;

namespace robot{

class AbstractTask: public Node, protected CommandSource, protected NotificationHandler, public NotificationSource, public ObjectWrap{
public:
    AbstractTask(const string& name, int _rank=0):Node(name), taskKilled(false), state(TaskState::SUSPENDED),rank(_rank){}

    bool passMessage(Message* message);
    bool isSubscribed(Notification *message);
    TaskState getTaskState() const;

    //task manager will call this when task's state changes from READY to RUNNING
    bool runTask();
    //task manager will call this when task changes from RUNNING to READY
    bool pauseTask();
    //task manager will call this when task is terminated, task thread will be killed
    void stop();

    //ovo bi trebao samo manager da poziva. Potrebno je atomicno promenuti stanje i updateovati heap.
    void setState(TaskState _state, bool force=false);
    void updateState(TaskState _state);
    bool getColor(StartMessage::Color& color);

    void registerManager(TaskManagerInterface* manager);
    TaskManagerInterface* getHandler() const;

    int getRank() const;
    string getLocalState(const string& key);
    void setLocalState(const string& key, const string& value);
protected:
    struct Instruction{
        enum Type{
            MESSAGE,
            START,
            STOP,
            KILL
        };

        Instruction(Message* _message):message(_message),type(MESSAGE),messageProcesed(NULL),processedLock(NULL){}
        Instruction(Type _type):type(_type){}

        Message* message;
        Type type;

        mutex* processedLock;
        bool* processed;
        condition_variable* messageProcesed;
    };

    Instruction fetchInstruction();
    void main();

    //this will be called when task's thread is created and it should be used to subsribe for desired events
    virtual void onCreate()=0;
    //this will be called on match start to notify tasks of current color
    virtual void onSetup(StartMessage::Color matchColor)=0;
    //this will notify task that it has been started (changed state to RUNNING)
    virtual void onRun()=0;
    //this will notify task that it has been stopped (changed state from RUNNING)
    virtual void onPause()=0;
    //this will notify task that it has been stopped (changed state from RUNNING)
    virtual void onDestroy()=0;

    //Override of CommandSource method, checking if task is premitted to send commands
    int sendCommand(Command* command, responseCallback success, responseCallback error, responseCallback progress=NULL);
private:
    mutex queueLock;
    condition_variable queueNotEmpty;
    queue<Instruction> instructionQueue;

    TaskState state;
    bool taskKilled;

    TaskManagerInterface* handler;
    int rank;
    mutex localStateLock;
    map<string, string> localState;
};

struct TaskExecutionException: public std::exception{
    TaskExecutionException(const string& _reason):reason(_reason){}

    const char* what() const throw (){
        return reason.c_str();
    }

private:
    string reason;
};

}

#endif //_ABSTRACTTASK_H
