//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : Timer_windows2.cpp
//==============================================================================


#include <Windows.h>
#include "Timer.h"
#include <mmsystem.h>

namespace Grape
{
    //==============================================================================
    /// \class TimerP
    /// \brief private implementation for Timer class
	/// Implements using Windows multimedia timer
    //==============================================================================
    class GRAPETIMING_DLL_API TimerP
	{
	public:
        TimerP() throw (Exception);
        ~TimerP() throw();
        void start(long long ns, bool isOneShot = false) throw(Exception);
        void stop() throw();
		static void CALLBACK timerCallback(UINT timerId, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	public:
        LARGE_INTEGER _countsPerSec;
        LARGE_INTEGER _startCount;
        INT _resolution;
        unsigned long long _periodNs;
        bool _isRunning;
        UINT _timerId;
        HANDLE _hndTimerEvent;
	};

	//==============================================================================
    void TimerP::timerCallback(UINT timerId, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
	//==============================================================================
	{
        timerId = timerId;
        dw1 = dw1;
        dw2 = dw2;
        uMsg = uMsg;

		if( dwUser )
		{
			HANDLE hEvent = (HANDLE)dwUser;
			SetEvent(hEvent);
		}
	}

	//==============================================================================
    TimerP::TimerP() throw(Exception)
	//==============================================================================
    : _periodNs(0),
    _isRunning(false)
	{
        QueryPerformanceFrequency(&_countsPerSec);


		// set max available resolution
		TIMECAPS tc;
		if( MMSYSERR_NOERROR != timeGetDevCaps(&tc,sizeof(TIMECAPS)) )
		{
            throw Exception(GetLastError(), "[TimerP::TimerP (timeGetDevCaps)]");
		}

        _resolution = std::min<UINT>( std::max<UINT>(tc.wPeriodMin, 0), tc.wPeriodMax );
        if( MMSYSERR_NOERROR != timeBeginPeriod(_resolution) )
		{
            throw Exception(GetLastError(), "[TimerP::TimerP (timeBeginPeriod)]");
		}

		// event to trigger timer ticks
        _hndTimerEvent = CreateEvent(NULL, FALSE /*auto reset*/, FALSE /*initially non signalled*/, NULL);
        if( NULL == _hndTimerEvent )
		{
            throw Exception(GetLastError(), "[TimerP::TimerP (CreateEvent)]");
		}
	}

	//------------------------------------------------------------------------------
    TimerP::~TimerP() throw()
	//------------------------------------------------------------------------------
	{
		stop();
        CloseHandle(_hndTimerEvent);
	}

	//------------------------------------------------------------------------------
    void TimerP::start(long long ns, bool isOneShot) throw(Exception)
	//------------------------------------------------------------------------------
	{
        if( _isRunning )
		{
			return;
		}
		
		if( ns < 0 ) { ns = 0; }
		UINT ms = (UINT)(ns/1000000LL);

		// arm the timer
        UINT fuEvent = ( isOneShot ? TIME_ONESHOT : TIME_PERIODIC);
        _timerId = timeSetEvent( ms, _resolution, timerCallback, (DWORD_PTR)_hndTimerEvent, fuEvent);
        if( _timerId== NULL)
		{
            throw Exception(GetLastError(), "[TimerP::start (timeSetEvent)]");
		}
        QueryPerformanceCounter( &_startCount );
        _periodNs = ns;
        _isRunning = true;
	}

	//------------------------------------------------------------------------------
    void TimerP::stop() throw()
	//------------------------------------------------------------------------------
	{
        if( !_isRunning )
		{
			return;
		}
		
        timeKillEvent(_timerId);
        timeEndPeriod(_resolution);

        _isRunning = false;
	}

	//==============================================================================
    Timer::Timer(int prio) throw(Exception, std::bad_alloc)
	//==============================================================================
    : _pImpl(new TimerP )
	{
        prio = prio;
	}

	//------------------------------------------------------------------------------
    Timer::~Timer()
	//------------------------------------------------------------------------------
	{
        delete _pImpl;
	}

	//------------------------------------------------------------------------------
    int Timer::getNotificationSignal() const
	//------------------------------------------------------------------------------
	{
		return -1;
	}
	
	//------------------------------------------------------------------------------
    long long Timer::getResolution() const throw(Exception)
	//------------------------------------------------------------------------------
	{
        return (_pImpl->_resolution * 1000000LL);
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
        return (WaitForSingleObject(_pImpl->_hndTimerEvent, INFINITE) == WAIT_OBJECT_0);
	}

	//------------------------------------------------------------------------------
    bool Timer::timedWait(long long ns) const throw()
	//------------------------------------------------------------------------------
	{
		if( ns < 0 ){ ns = 0; }
		long long ms = ns/1000000LL;
        return (WaitForSingleObject(_pImpl->_hndTimerEvent, (DWORD)ms) == WAIT_OBJECT_0 );
	}

	//------------------------------------------------------------------------------
    void Timer::forceTimerTick() const throw()
	//------------------------------------------------------------------------------
	{
        SetEvent(_pImpl->_hndTimerEvent);
	}

	//------------------------------------------------------------------------------
    long long Timer::getNumTicks() const throw(Exception)
	//------------------------------------------------------------------------------
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter( &now );

        long long int nTicks = (1000000000LL * (now.QuadPart - _pImpl->_startCount.QuadPart))/_pImpl->_countsPerSec.QuadPart;
        nTicks /= _pImpl->_periodNs;

		return nTicks;
	}

} // Grape
