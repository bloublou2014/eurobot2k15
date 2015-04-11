#ifndef _PIPETASK_H
#define _PIPETASK_H

#include <boost/thread/mutex.hpp>

#include "tasks/AbstractTask.h"
#include "executors/msg/ActuatorCommand.h"
#include "executors/msg/MotionCommand.h"

// fifo

#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace  servo;
using namespace executor;
using namespace std;
using namespace motion;
using boost::mutex;
using std::queue;

namespace robot{

class PipeTask: public AbstractTask{
private:
    enum state_type{
        READ_WORD,
        READ_PIPE,
        EXEC_NAME,
        EXEC_OPTION,
        EXEC_SEND,
        EXEC_ARGV0,
        EXEC_ARGV1,
        CLEAR_PIPE
    };

public:
    PipeTask(const string &name):AbstractTask(name){}
protected:
    void initScript();
    void startScript();
    void stopScript();
    void init();

private:
    void commandResponseSuccess(Command* _command);
    void commandResponseError(Command* _command);
    void readLine(Notification *testNotification);

    std::map<string, Executors> executorMap ;
    std::map<string, ActuatorType> actionMap;

    std::ifstream fifo;
    std::string line;
    bool done = false;
    bool shouldStop = false;
    boost::mutex fifoLock;
    state_type state = CLEAR_PIPE; // promenjeno posto kada je ubije aktuatorska onda zna ostati u pipeu :) bilo READ_PIPE;
    state_type returnState = EXEC_NAME;
    Executors executor;
    ActuatorType action;
    MotionCommand::MotionType motionAction;
    int argv;
    int argv1;

    queue<string> wordQueue; // queue<string*>
    boost::mutex queueLock;
    string word;
};

}


#endif // _PIPETASK_H
