#ifndef ABSTRACTACTUATOR_H
#define ABSTRACTACTUATOR_H

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <queue>
#include <map>

#include "executors/AbstractExecutor.h"
//#include "ServoCommand.h"
#include "drivers/actuators/Servos.h"

using namespace robot;
using std::queue;
using boost::mutex;

namespace actuators{

class AbstractActuator: public AbstractExecutor{
public:
    AbstractActuator():AbstractExecutor(NAME), currentCommand(NULL){}

    static string NAME;
    void init();
    void stop();

protected:
    main();

private:
    mutex commandQueueLock;
    queue<Command*> commandsToProcess;



};


}


#endif // ABSTRACTACTUATOR_H
