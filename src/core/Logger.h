#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>
#include <iostream>

using namespace::std;

namespace robot{

class Logger{
public:
    Logger();
    Logger(const string& _name);
protected:
    void setName(const string& _name);

    void debug(const string& message);
    void info(const string& message);
    void warning(const string& message);
    void error(const string& message);
private:
    string name;
};

}

#endif //_LOGGER_H
