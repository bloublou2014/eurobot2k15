#ifndef _NODE_H
#define _NODE_H

#include <string>
#include "boost/thread.hpp"
#include "boost/date_time.hpp"

#include "messages/Notification.h"

using namespace std;
using boost::thread;

namespace robot{

class Node{
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
private:
    string name;
    boost::thread thread;
};

}

#endif //_NODE_H
