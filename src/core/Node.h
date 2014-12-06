#ifndef _NODE_H
#define _NODE_H

#include <string>

#include "messages/Notification.h"

using namespace std;

namespace robot{

class Node{
public:
    Node(const string& _name):name(_name){}
    string getName() const;

    virtual void init();
    virtual void start();
    virtual void stop();

protected:
    void setName(const string& _name);
private:
    string name;
};

}

#endif //_NODE_H
