#include "SleepTimer.h"

namespace helper{

SleepTimer::SleepTimer(TimerCallback *_callback, int _milliseconds, void *_obj):callback(_callback), thread(NULL),
    finished(false){
    start(_milliseconds, _obj);
}

bool SleepTimer::start(int _milliseconds, void* _obj){
    finished=false;
    milliseconds=_milliseconds;
    obj=_obj;
    if (thread){
        delete thread;
    }
    thread=new boost::thread(&SleepTimer::countdown,this);
    return true;
}

void SleepTimer::countdown(){
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::milliseconds(milliseconds));
    t.async_wait(boost::bind(&TimerCallback::onTimeout, callback, boost::asio::placeholders::error, obj));
    io.run();
    finished=true;
}

SleepTimer::~SleepTimer(){
    if (thread){
        delete thread;
    }
}

bool SleepTimer::isFinished(){
    return finished;
}

}
