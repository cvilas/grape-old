//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : Timer_unix.cpp
//==============================================================================

#include "core/posix.h"
#include "Timer.h"
#include <iostream>
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
        explicit TimerP(int prio=0) throw(Exception); /* prio between 0 and RTSIG_MAX */
        ~TimerP() throw();
        void start(long long ns, bool isOneShot) throw(Exception);
        void stop() throw(Exception);
        
        int             _signal;
        timer_t         _timerId;
        clockid_t       _clockId;
        sigset_t        _sigSet;
        sigset_t        _sigOldSet;
        struct itimerspec _period;
        struct timespec _startTime;
        static const long long _NANO = 1000000000LL;
    };

    //==============================================================================
    TimerP::TimerP(int prio) throw(Exception)
    //==============================================================================
    :   _signal(SIGRTMAX - prio),
        _timerId(0),
        _clockId(CLOCKID)
    {
        if( _signal < SIGRTMIN ) _signal = SIGRTMIN;
        if( _signal > SIGRTMAX ) _signal = SIGRTMAX;

        struct sigaction sa;

        // is anyone else using this signal? we want exclusivity
        if( sigaction(_signal, NULL, &sa) < 0 )
        {
            throw Grape::Exception(errno, "[TimerP::TimerP (sigaction)]");
        }
        if( (sa.sa_sigaction != NULL) || (sa.sa_handler != NULL) )
        {
            throw Grape::Exception(errno, "[TimerP::TimerP] Real-time signal is already in use");
        }

        // setup notification of timer expiry
        struct sigevent sigev;
        sigev.sigev_notify = SIGEV_SIGNAL;              // notify me this way
        sigev.sigev_signo = _signal;
        sigev.sigev_value.sival_ptr = (void*)this;      // so I know it is indeed me who sent the signal when I receive it
        
        if( 0 != sigemptyset(&_sigSet) )
        {
            throw Grape::Exception(errno, "[TimerP::TimerP (sigemptyset)]");
        }
        if( 0 != sigaddset(&_sigSet, _signal) )         // we only want to wait on my signal
        {
            throw Grape::Exception(errno, "[TimerP::TimerP (sigaddset)]");
        }
        
        // create the timer
        if( 0 != timer_create(_clockId, &sigev, &_timerId) )
        {
            throw Grape::Exception(errno, "[TimerP::TimerP (timer_create)]");
        }

        // Note 1: posix says signal handlers have priority over sigwaitinfo. So
        // we got to make sure handlers can't catch this signal.
        // Mask the signal for this thread
        int rc = pthread_sigmask(SIG_BLOCK, &_sigSet, &_sigOldSet);
        //int rc = sigprocmask(SIG_BLOCK, &sigSet_, &sigOldSet_);

        if( rc != 0 )
        {
            throw Grape::Exception(rc, "[TimerP::TimerP (pthread_sigmask)]");
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
        
        // restore original signal mask. See Note 1.
        sigprocmask(SIG_SETMASK, &_sigOldSet, NULL);
    }
    
    //------------------------------------------------------------------------------
    void TimerP::start(long long ns, bool isOneShot) throw(Exception)
    //------------------------------------------------------------------------------
    {        
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
            throw Grape::Exception(errno, "[TimerP::start (timer_settime)]");
        }
        
        if( clock_gettime(CLOCKID, &_startTime) < 0 )
        {
            throw Grape::Exception(errno, "[TimerP::start (clock_gettime)]");
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
            throw Grape::Exception(errno, "[TimerP::stop (timer_settime)]");
        }
    }
    
    //==============================================================================
    Timer::Timer(int prio) throw(Exception, std::bad_alloc)
    //==============================================================================
    : _pImpl( new TimerP(prio) )
    {
    }

    //------------------------------------------------------------------------------
    Timer::~Timer() throw()
    //------------------------------------------------------------------------------
    {
        delete _pImpl;
    }

    //------------------------------------------------------------------------------
    int Timer::getNotificationSignal() const throw()
    //------------------------------------------------------------------------------
    {
        return _pImpl->_signal;
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
    bool Timer::wait() const throw()
    //------------------------------------------------------------------------------
    {        
        siginfo_t info;
        if(  (sigwaitinfo(&_pImpl->_sigSet, &info) < 0)
             || (info.si_value.sival_ptr != _pImpl)
             || (info.si_signo != _pImpl->_signal) )
        {
            if(info.si_value.sival_ptr != _pImpl)
            {
#ifdef DEBUG
                fprintf(stderr, "[Timer::wait] Woken up by someone else\n");
#endif
            }
            else if (info.si_signo != _pImpl->_signal)
            {
#ifdef DEBUG
                fprintf(stderr, "[Timer::wait] Signal %d received waiting for signal %d.\n", info.si_signo, pImpl_->_signal);
#endif
            }
            return false;
        }

        // ok, if we unblocked due to my timer and my signal
        return true;
        
        // Notes:
        // error if sigNo < 0
        // info.si_code == SI_TIMER; // if signal sent due to timer expiry
        // info.si_ptr == pImpl_; // our timer
        // info.si_signo == pImpl_->_signal; // our signal
        
    }
    
    //------------------------------------------------------------------------------
    bool Timer::timedWait(long long ns) const throw()
    //------------------------------------------------------------------------------
    {
        struct timespec waitTime;
        waitTime.tv_sec = (long)(ns/TimerP::_NANO);
        waitTime.tv_nsec = (long)(ns%TimerP::_NANO);

        siginfo_t info;      
        
        if( (sigtimedwait(&(_pImpl->_sigSet), &info, &waitTime) < 0 )
             || (info.si_value.sival_ptr != _pImpl)
             || (info.si_signo != _pImpl->_signal) )
        {
            if(info.si_value.sival_ptr != _pImpl)
            {
                fprintf(stderr, "[Timer::timedWait] Woken up by someone else.\n");
            }
            else if (info.si_signo != _pImpl->_signal)
            {
                fprintf(stderr, "[Timer::timedWait] Signal %d received waiting for signal %d\n", info.si_signo, _pImpl->_signal);
            }
            return false;
        }       

        // ok, if we unblocked due to my timer and my signal
        return true;
    }

    //------------------------------------------------------------------------------
    void Timer::forceTimerTick() const throw()
    //------------------------------------------------------------------------------
    {
        if( 0 != kill(0, _pImpl->_signal) )
        {
            fprintf(stderr, "[Timer::forceTimerTick (kill)] Error %d (%s)\n", errno, strerror(errno));
        }
    }
    
    //------------------------------------------------------------------------------
    long long Timer::getResolution() const throw(Exception)
    //------------------------------------------------------------------------------
    {
        struct timespec res;
        if( clock_getres(CLOCKID, &res) < 0 )
        {
            throw Grape::Exception(errno, "[TimerP::getResolution (clock_getres)]");
            // CLOCKID is not supported
        }
        return (res.tv_sec * TimerP::_NANO) + res.tv_nsec;
        
    }
    
    //------------------------------------------------------------------------------
    long long int Timer::getNumTicks() const throw(Exception)
    //------------------------------------------------------------------------------
    { 
        itimerspec period;
        timespec now, elapsed, startTime;
        
        memcpy(&startTime, &(_pImpl->_startTime), sizeof(startTime));
        memcpy(&period, &(_pImpl->_period), sizeof(period));

        if( clock_gettime(CLOCKID, &now) < 0 )
        {
            throw Grape::Exception(errno, "[TimerP::getNumTicks (clock_gettime)]");
        }

        if( !Grape::timespecDiff(now, startTime, elapsed) )
        {
            throw Grape::Exception(-1, "[TimerP::getOverruns] Current time < unblock time");
        }
        long long ns = elapsed.tv_sec * TimerP::_NANO + elapsed.tv_nsec;
        return ns/(period.it_value.tv_sec * TimerP::_NANO + period.it_value.tv_nsec);
    }

} // Grape
