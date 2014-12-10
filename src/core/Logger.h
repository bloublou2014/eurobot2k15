#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>

using namespace::std;

namespace robot{

class Logger{
public:
protected:
    void debug(const string& message);
    void info(const string& message);
    void warning(const string& message);
    void error(const string& message);
private:
};

}

#endif //_LOGGER_H
