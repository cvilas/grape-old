//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : Timer_windows2.cpp
//
// Copyright (c) 2012, Vilas Chitrakaran
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
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
        bool wait(DWORD ms)throw(Exception);
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

    //------------------------------------------------------------------------------
    bool TimerP::wait(DWORD ms)throw(Exception)
    //------------------------------------------------------------------------------
    {
        DWORD ret = WaitForSingleObject(_hndTimerEvent, ms);
        if( ret == WAIT_FAILED )
        {
            throw Exception(GetLastError(), "[TimerP::wait(WaitForSingleObject)]");
        }

        return ( ret == WAIT_OBJECT_0 );
    }

    //==============================================================================
    Timer::Timer() throw(Exception, std::bad_alloc)
	//==============================================================================
    : _pImpl(new TimerP )
	{
	}

	//------------------------------------------------------------------------------
    Timer::~Timer()
	//------------------------------------------------------------------------------
	{
        delete _pImpl;
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
    bool Timer::wait() const throw(Exception)
	//------------------------------------------------------------------------------
	{
        return _pImpl->wait(INFINITE);
	}

	//------------------------------------------------------------------------------
    bool Timer::timedWait(long long ns) const throw(Exception)
	//------------------------------------------------------------------------------
	{
        if( ns < 0 ) { ns = 0; }
        long long ms = ns/1000000LL;
        return _pImpl->wait((DWORD)ms);
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
