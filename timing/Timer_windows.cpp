//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : Timer_windows.cpp
//==============================================================================

#include "Timer.h"
#include <iostream>

namespace Generic 
{
    //==============================================================================
    /// \class TimerP
    /// \brief private implementation for Timer class
	/// Implements using Windows Queue timers
	/// Note: Prefer the implementation in Timer_private_win2.cpp. The newer Queue timers 
	/// are reportedly more innacurate compared to the 'obsolete' mm timers. 
    //==============================================================================
    class GRAPETIMING_DLL_API TimerP
	{
	public:
        TimerP() throw (Exception);
        ~TimerP();
        void start(unsigned long long ns, bool isOneShot = false) throw(Exception);
        void stop() throw(Exception);
		static void timerCallback(PVOID lpParam, BOOLEAN TimerOrWaitFired);
	public:
        LARGE_INTEGER _countsPerSec;
        LARGE_INTEGER _startCount;
        unsigned long long _periodNs;
        bool _isRunning;
        HANDLE _hndTimer;
        HANDLE _hndTimerQueue;
        HANDLE _hndTimerEvent;
	};

	//==============================================================================
    void TimerP::timerCallback(PVOID lpParam, BOOLEAN TimerOrWaitFired)
	//==============================================================================
	{
		if( lpParam == NULL )
		{
            std::cerr << "[TimerP::timerCallback] Unexpected timer signal" << std::endl;
			return;
		}

		if( TimerOrWaitFired ) // actually, always TRUE for timers
		{
			if( !SetEvent(*(PHANDLE*)(lpParam)/*hndTimerEvent_*/) )
			{
                std::cerr << "[TimerP::timerCallback] Can't signal timer event" << std::endl;
			}
		}
	}

	//==============================================================================
    TimerP::TimerP()
	//==============================================================================
    : _periodNs(0),
    _isRunning(false)
	{
		timeBeginPeriod(1);
        QueryPerformanceFrequency(&_countsPerSec);
	}

	//------------------------------------------------------------------------------
    TimerP::~TimerP()
	//------------------------------------------------------------------------------
	{
		timeEndPeriod(1);
	}

	//------------------------------------------------------------------------------
    void TimerP::start(unsigned long long ns, bool isOneShot) throw(Exception)
	//------------------------------------------------------------------------------
	{
        if( _isRunning )
		{
			return;
		}
		
		unsigned long long ms = ns/1000000ULL;

		// create the timer queue
        _hndTimerQueue = CreateTimerQueue();
        if( _hndTimerQueue == NULL )
		{
            throw Exception(GetLastError(), "[TimerP::start (CreateTimerQueue)]");
		}

		// we will use an event to notify timer expiry
        _hndTimerEvent = CreateEvent(NULL, FALSE /*auto reset*/, FALSE /*initially non signalled*/, NULL);
        if( NULL == _hndTimerEvent )
		{
            throw Exception(GetLastError(), "[TimerP::start (CreateEvent)]");
		}

		// arm the timer
        if( !CreateTimerQueueTimer(&_hndTimer, _hndTimerQueue,
            (WAITORTIMERCALLBACK)timerCallback, &_hndTimerEvent,
			(DWORD)ms, (DWORD)(isOneShot?(0):(ms)), 0) )
		{
            throw Exception(GetLastError(), "[TimerP::start (CreateTimerQueueTimer)]");
		}
        QueryPerformanceCounter( &_startCount );
        _periodNs = ns;
        _isRunning = true;
	}

	//------------------------------------------------------------------------------
    void TimerP::stop() throw(Exception)
	//------------------------------------------------------------------------------
	{
        if( !_isRunning )
		{
			return;
		}
        _isRunning = false;

		// delete the timer and all associated handles
        if( !DeleteTimerQueueTimer(_hndTimerQueue, _hndTimer, _hndTimerEvent) )
		{
            throw Exception(GetLastError(), "[TimerP::stop (DeleteTimerQueueTimer)]");
		}
	}

	//==============================================================================
    Timer::Timer(unsigned int prio) throw(Exception, std::bad_alloc)
	//==============================================================================
    : pImpl_(new TimerP ), nTicksAtLastWait_(0)
	{
	}

	//------------------------------------------------------------------------------
    Timer::~Timer()
	//------------------------------------------------------------------------------
	{
		lock_.lock();
		delete pImpl_;
		pImpl_ = NULL;
		lock_.unlock();
	}

	//------------------------------------------------------------------------------
    int Timer::getNotificationSignal() const
	//------------------------------------------------------------------------------
	{
		return -1;
	}
	
	//------------------------------------------------------------------------------
    unsigned long long Timer::getResolution() throw(Exception)
	//------------------------------------------------------------------------------
	{
		return 1000000; // 1ms
	}

	//------------------------------------------------------------------------------
    void Timer::start(unsigned long long ns, bool isOneShot) throw(Exception)
	//------------------------------------------------------------------------------
	{
		lock_.lock();
		if( pImpl_ ) pImpl_->start(ns, isOneShot);
		lock_.unlock();
		nTicksAtLastWait_ = 0;
	}
	
	//------------------------------------------------------------------------------
    void Timer::stop() throw(Exception)
	//------------------------------------------------------------------------------
	{
		lock_.lock();
		if( pImpl_ ) pImpl_->stop();
		lock_.unlock();
	}
	
	//------------------------------------------------------------------------------
    int Timer::wait()
	//------------------------------------------------------------------------------
	{
		HANDLE hndTimerEvent = NULL;
		lock_.lock();
		if( pImpl_ ) hndTimerEvent = pImpl_->hndTimerEvent_;
		lock_.unlock();

		/// wait for timer
		int nOverflows = 0;
		if( hndTimerEvent != NULL )
		{
			unsigned long long nTicks = getNumTicks();
			int nOverflows = (int)(nTicks - nTicksAtLastWait_);
			if( nOverflows < 0 ) nOverflows = 0; // gosh!
			if( nOverflows > 0 )
			{
                //std::cerr << "[Timer::wait] WARNING - Timer overflowed " << nOverflows << " times." << std::endl << std::flush;
			}
			if( WaitForSingleObject(hndTimerEvent, INFINITE) != WAIT_OBJECT_0 )
			{
				return -1;
			}
			nTicksAtLastWait_ = nTicks+1;
		}

		return nOverflows;
	}

	//------------------------------------------------------------------------------
    int Timer::timedWait(unsigned long long ns)
	//------------------------------------------------------------------------------
	{
		unsigned long long ms = ns/1000000ULL;

		HANDLE hndTimerEvent = NULL;
		lock_.lock();
		if( pImpl_ ) hndTimerEvent = pImpl_->hndTimerEvent_;
		lock_.unlock();

		/// wait for timer
		int nOverflows = 0;
		if( hndTimerEvent != NULL )
		{
			unsigned long long nTicks = getNumTicks();
			nOverflows = (int)(nTicks - nTicksAtLastWait_);
			if( nOverflows < 0 ) nOverflows = 0; // gosh!
			if( nOverflows > 0 )
			{
                //std::cerr << "[Timer::wait] WARNING - Timer overflowed " << nOverflows << " times." << std::endl << std::flush;
			}
			if( WaitForSingleObject(hndTimerEvent, (DWORD)ms) != WAIT_OBJECT_0 )
			{
				return -1;
			}
			nTicksAtLastWait_ = nTicks+1;
		}

		return nOverflows;
	}

	//------------------------------------------------------------------------------
    void Timer::forceTimerTick()
	//------------------------------------------------------------------------------
	{
		lock_.lock();
		if( pImpl_ ) 
		{
			SetEvent(pImpl_->hndTimerEvent_);
		}
		lock_.unlock();
	}

	//------------------------------------------------------------------------------
    long long Timer::getNumTicks() throw(Exception)
	//------------------------------------------------------------------------------
	{
		static long long pre;
		LARGE_INTEGER now;
		QueryPerformanceCounter( &now );

		long long int nTicks = 0;

		lock_.lock();
		if( pImpl_ )
		{
			nTicks = (1000000000LL * (now.QuadPart - pImpl_->startCount_.QuadPart))/pImpl_->countsPerSec_.QuadPart;
			nTicks /= pImpl_->periodNs_;
		}
		lock_.unlock();

		return nTicks;
	}

};// namespace Generic
