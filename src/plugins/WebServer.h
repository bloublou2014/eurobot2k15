#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#include <string>
#include <list>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <mongoose/Server.h>
#include <mongoose/WebController.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

using std::string;
using std::list;
using boost::thread;
using boost::condition_variable;
using boost::unique_lock;
using boost::mutex;
using namespace Mongoose;

namespace robot {

class WebServer{
public:
    WebServer(int _port):server(_port),shouldStop(false){}
    void start();
    void stop();
protected:
    void main();
private:
    boost::thread thread;
    boost::mutex stopLock;
    boost::condition_variable stopRequest;
    Server server;
    bool shouldStop;
};

}

#endif  //_WEBSERVER_H
