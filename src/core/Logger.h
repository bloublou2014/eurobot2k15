#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>

using namespace::std;

namespace robot{

class Logger{
public:
    virtual void debug(const string& message)=0;
    virtual void info(const string& message)=0;
    virtual void warning(const string& message)=0;
    virtual void error(const string& message)=0;
private:
};

}

#endif //_LOGGER_H
