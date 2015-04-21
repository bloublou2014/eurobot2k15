#ifndef SLEEPTIMER_H
#define SLEEPTIMER_H

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/atomic.hpp>

using namespace boost::asio;

namespace helper{

class TimerCallback{
public:
    virtual void onTimeout(const boost::system::error_code &e, boost::asio::deadline_timer* t, void* obj, bool repeat)=0;
};

class SleepTimer
{
public:
    SleepTimer(TimerCallback* _callback, int _milliseconds, void* _obj, bool _repeat=false);
    bool start(int _milliseconds, void* _obj);
    bool isFinished();

    ~SleepTimer();
private:
    void countdown();

    TimerCallback* callback;
    int milliseconds;

    bool repeat;
    boost::atomic<bool> finished;
    void* obj;
    boost::thread* thread;
};

}

#endif // SLEEPTIMER_H
