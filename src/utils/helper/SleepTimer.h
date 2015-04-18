#ifndef SLEEPTIMER_H
#define SLEEPTIMER_H

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


using namespace boost::asio;

namespace helper{

class TimerCallback{
public:
    virtual void onTimeout(const boost::system::error_code &e)=0;
};

class SleepTimer
{
public:
    SleepTimer(TimerCallback* _callback, int _milliseconds);
    bool start(int _milliseconds);
    bool isFinished();

    ~SleepTimer();
private:
    void countdown();

    TimerCallback* callback;
    int milliseconds;
    bool finished;

    boost::thread* thread;
};

}

#endif // SLEEPTIMER_H
