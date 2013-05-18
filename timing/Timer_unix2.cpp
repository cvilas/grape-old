//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : Timer_unix.cpp
//==============================================================================

#include "core/posix.h"
#include "Timer.h"
#include <sys/time.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h> // for fprintf

namespace Grape
{
    static const clockid_t CLOCKID = DEFAULT_CLOCK;

    //==============================================================================
    /// \class TimerP
    /// \brief private implementation for Timer class
    //==============================================================================
    class TimerP
    {
    public:
        static void TimerEventHandler(union sigval sv) throw(Exception);
        static const long long _NANO = 1000000000LL;
    public:
        explicit TimerP() throw(Exception); /* prio between 0 and RTSIG_MAX */
        ~TimerP() throw();
        void start(long long ns, bool isOneShot) throw(Exception);
        void stop() throw(Exception);
        long long getNumTicks() throw(Exception);
        bool wait(long long ns);
        timer_t             _timerId;
        clockid_t           _clockId;
        struct itimerspec   _period;
        long long           _nTicks;
        pthread_mutex_t     _lock;
        pthread_cond_t      _condVar;
        bool                _isTick;
    };

    //==============================================================================
    TimerP::TimerP() throw(Exception)
    //==============================================================================
    :   _timerId(0),
        _clockId(CLOCKID),
        _nTicks(0),
        _lock(PTHREAD_MUTEX_INITIALIZER),
        _condVar(PTHREAD_COND_INITIALIZER),
        _isTick(false)
    {
         // setup notification of timer expiry
        struct sigevent sigev;
        sigev.sigev_notify = SIGEV_THREAD;                          // notify me this way
        sigev.sigev_notify_function = TimerP::TimerEventHandler;    // through this
        sigev.sigev_value.sival_ptr = (void*)this;                  // with this information
        sigev.sigev_notify_attributes = NULL;

        // create the timer
        if( 0 != timer_create(_clockId, &sigev, &_timerId) )
        {
            throw Exception(errno, "[TimerP::TimerP (timer_create)]");
        }
    }

    //------------------------------------------------------------------------------
    TimerP::~TimerP() throw()
    //------------------------------------------------------------------------------
    {
        if( 0 != timer_delete(_timerId) )
        {
            int e = errno;
            fprintf(stderr, "[TimerP::~TimerP (timer_delete)] Error %d (%s)", e, strerror(e));
        }
        _timerId = 0;
    }

    //------------------------------------------------------------------------------
    void TimerP::start(long long ns, bool isOneShot) throw(Exception)
    //------------------------------------------------------------------------------
    {
        _nTicks = 0;
        _isTick = false;

        long sec = (long)(ns/_NANO);
        long nsec = (long)(ns - sec * _NANO);

        // first tick at...
        _period.it_value.tv_sec = sec;
        _period.it_value.tv_nsec = nsec;

        //..and thereafter, regular ticks at
        _period.it_interval.tv_sec = isOneShot ? 0 : sec;
        _period.it_interval.tv_nsec = isOneShot ? 0 : nsec;

        if( timer_settime(_timerId, 0/*flags*/, &_period, NULL /* oldperiod*/) < 0 )
        {
            throw Exception(errno, "[TimerP::start (timer_settime)]");
        }
    }

    //------------------------------------------------------------------------------
    void TimerP::stop() throw(Exception)
    //------------------------------------------------------------------------------
    {
        //disarm...
        _period.it_value.tv_sec = 0;
        _period.it_value.tv_nsec = 0;

        if( timer_settime(_timerId, 0/*flags*/, &_period, NULL /* oldperiod*/) < 0 )
        {
            throw Exception(errno, "[TimerP::stop (timer_settime)]");
        }
    }

    //------------------------------------------------------------------------------
    long long TimerP::getNumTicks() throw(Exception)
    //------------------------------------------------------------------------------
    {
        long long n = 0;

        int status = pthread_mutex_lock(&_lock);
        if( status != 0 ) { throw Exception(status, "[TimerP::getNumTicks(pthread_mutex_lock)]"); }

        n = _nTicks;

        status = pthread_mutex_unlock(&_lock);
        if( status != 0 ) { throw Exception(status, "[TimerP::getNumTicks(pthread_mutex_unlock)]"); }

        return n;
    }

    //------------------------------------------------------------------------------
    bool TimerP::wait(long long ns)
    //------------------------------------------------------------------------------
    {
        struct timeval now;
        struct timespec absTime;

        // compute timeout in absolute time
        if( ns > 0)
        {
            gettimeofday(&now, NULL);
            long long absTimeNs = (now.tv_sec * _NANO) + (now.tv_usec * 1000) + ns;

            absTime.tv_sec = (long)(absTimeNs/_NANO);
            absTime.tv_nsec = (long)(absTimeNs - absTime.tv_sec * _NANO);
        }
        else
        {
            absTime.tv_sec = INT_MAX;
            absTime.tv_nsec = LONG_MAX;
        }

        bool ticked = false;

        int status = pthread_mutex_lock(&_lock);
        if( status != 0 ) { throw Exception(status, "[TimerP::wait(pthread_mutex_lock)]"); }

        while( !_isTick )
        {
            status = pthread_cond_timedwait(&_condVar, &_lock, &absTime);
            if( status == ETIMEDOUT )
            {
                break;
            }
            else if( status != 0 )
            {
                throw Exception(status, "[TimerP::wait(pthread_cond_timedwait)]");
            }
        }

        ticked = _isTick;
        _isTick = false;

        status = pthread_mutex_unlock(&_lock);
        if( status != 0 ) { throw Exception(status, "[TimerP::wait(pthread_mutex_unlock)]"); }

        return ticked;
    }

    //------------------------------------------------------------------------------
    void TimerP::TimerEventHandler(sigval sv) throw(Exception)
    //------------------------------------------------------------------------------
    {
        int status = 0;
        TimerP* pImpl = (TimerP*)(sv.sival_ptr);

        status = pthread_mutex_lock(&pImpl->_lock);
        if( status != 0 ) { throw Exception(status, "[TimerP::TimerEventHandler(pthread_mutex_lock)]"); }

        pImpl->_nTicks += 1 + timer_getoverrun(pImpl->_timerId);
        pImpl->_isTick = true;

        status = pthread_cond_signal(&pImpl->_condVar);
        if( status != 0 ) { throw Exception(status, "[TimerP::TimerEventHandler(pthread_cond_signal)]"); }

        status = pthread_mutex_unlock(&pImpl->_lock);
        if( status != 0 ) { throw Exception(status, "[TimerP::TimerEventHandler(pthread_mutex_unlock)]"); }
    }

    //==============================================================================
    Timer::Timer() throw(Exception, std::bad_alloc)
    //==============================================================================
    : _pImpl( new TimerP )
    {
    }

    //------------------------------------------------------------------------------
    Timer::~Timer() throw()
    //------------------------------------------------------------------------------
    {
        delete _pImpl;
    }

    //------------------------------------------------------------------------------
    void Timer::start(long long ns, bool isOneShot) throw(Exception)
    //------------------------------------------------------------------------------
    {
        _pImpl->start(ns, isOneShot);
    }

    //------------------------------------------------------------------------------
    void Timer::stop() throw(Exception)
    //------------------------------------------------------------------------------
    {
        _pImpl->stop();
    }

    //------------------------------------------------------------------------------
    bool Timer::wait() const throw(Exception)
    //------------------------------------------------------------------------------
    {
        return _pImpl->wait(-1);
    }

    //------------------------------------------------------------------------------
    bool Timer::timedWait(long long ns) const throw(Exception)
    //------------------------------------------------------------------------------
    {
        return _pImpl->wait(ns);
    }

    //------------------------------------------------------------------------------
    void Timer::forceTimerTick() const throw()
    //------------------------------------------------------------------------------
    {
        sigval sv;
        sv.sival_ptr = _pImpl;
        try
        {
            TimerP::TimerEventHandler(sv);
        }
        catch(Exception &ex)
        {
            fprintf(stderr, "%s\n", ex.what());
        }
    }

    //------------------------------------------------------------------------------
    long long Timer::getResolution() const throw(Exception)
    //------------------------------------------------------------------------------
    {
        struct timespec res;
        if( clock_getres(CLOCKID, &res) < 0 )
        {
            throw Exception(errno, "[TimerP::getResolution (clock_getres)]");
            // CLOCKID is not supported
        }
        return (res.tv_sec * TimerP::_NANO) + res.tv_nsec;

    }

    //------------------------------------------------------------------------------
    long long int Timer::getNumTicks() const throw(Exception)
    //------------------------------------------------------------------------------
    {
        return _pImpl->getNumTicks();
    }

} // Grape
