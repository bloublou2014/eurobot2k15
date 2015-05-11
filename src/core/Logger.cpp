#include "Logger.h"

namespace robot{

Logger::Logger(){}

Logger::Logger(const string& _name):name(_name){}

void Logger::setName(const string& _name){
    name=_name;
}

void Logger::debug(const string& message){
    cout<<name<<" [D]: "<<message<<endl;
    //BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
}

void Logger::error(const string& message){
    cout<<name<<" [E]: "<<message<<endl;
    //BOOST_LOG_TRIVIAL(loggging::trivial::error)<<name<<": "<<message;
}

void Logger::warning(const string& message){
    //BOOST_LOG_TRIVIAL(loggging::trivial::warning)<<name<<": "<<message;
}

void Logger::info(const string& message){
    //BOOST_LOG_TRIVIAL(loggging::trivial::info)<<name<<": "<<message;
}

}
