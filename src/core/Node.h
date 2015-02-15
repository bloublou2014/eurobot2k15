#ifndef _NODE_H
#define _NODE_H

#include <string>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/log/trivial.hpp>

#include "Logger.h"
#include "messages/Notification.h"

using namespace std;
using boost::thread;

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

namespace robot{

class Node : public Logger{
public:
    Node(const string& _name):name(_name){}
    string getName() const;

    virtual void init();
    virtual void start();
    virtual void stop();

    void join();
protected:
    void setName(const string& _name);
    virtual void main()=0;

    void debug(const string& message);
    void error(const string& message);
    void warning(const string& message);
    void info(const string& message);
private:
    string name;
    boost::thread thread;
};

}

#endif //_NODE_H
