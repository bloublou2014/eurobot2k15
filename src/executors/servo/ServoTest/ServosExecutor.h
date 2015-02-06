#ifndef _SERVOSEXECUTOR_H
#define _SERVOSEXECUTOR_H

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <queue>
#include <map>

#include "executors/AbstractExecutor.h"
#include "ServoCommand.h"
#include "drivers/actuators/Servos.h"


using namespace robot;
using boost::mutex;
using std::queue;

namespace servo {
class ServoExecutor: public AbstractExecutor {
public:
    static string NAME;
    //ServoExecutor():AbstractExecutor(NAME),currentServoCommand(NULL){}
    ServoExecutor():AbstractExecutor(NAME),currentServoCommand(NULL){}
    void init();
    void stop();

    void processServoCommand(Command *command);
    void processGetServoState(Command* command);
protected:
    void main();
private:
    mutex commandQueueLock;
    queue<Command*> commandsToProcess;
    ServoCommand* getNextServoCommand();

    //mutex StateLock;
    bool shouldStop;
    ServoCommand* currentServoCommand;
    ServoDriver driver;

    typedef void( ServoExecutor::*servoCommandHandle)(ServoCommand* _servoCommand);
    typedef map<ServoCommand::ServoType, servoCommandHandle> ServoCommandHandleMap;
    ServoCommandHandleMap servoHandles;

    void rotateFor(ServoCommand* _servoCommand);
    void rotateToPosition(ServoCommand* _servoCommand);
    void setSpeed(ServoCommand* _servoCommand);

};

}

#endif // _SERVORSEXECUTOR_H
