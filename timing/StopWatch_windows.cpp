//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : StopWatch_windows.cpp
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

#include "StopWatch.h"
#include "Windows.h"
#include <errno.h>
#ifdef DEBUG
#include <iostream>
#endif

namespace grape
{
    /// Windows specific implementation class
    
    class GRAPETIMING_DLL_API StopWatchP
    {
    public:
        StopWatchP() throw() { zeroAll(); }
        ~StopWatchP() throw() {}
        void zeroAll() throw();
        
    private:
        StopWatchP(const StopWatchP&);                //!< prevent copy construction
        StopWatchP& operator=(const StopWatchP&);     //!< prevent assignment

    public:
        __int64 _startCount;
        __int64 _stopCount;
        __int64 _accumulatedCount;
        __int64 _countsPerSec;
        bool    _isRunning;
    }; // StopWatchP

    //==========================================================================
    void StopWatchP::zeroAll() throw()
    //==========================================================================
    {
        _isRunning = false;
        _startCount = 0;
        _stopCount = 0;
        _accumulatedCount = 0;
        QueryPerformanceFrequency((LARGE_INTEGER*)&_countsPerSec);
#ifdef DEBUG
        std::cout << "[StopWatchP] Counts per sec: " << _countsPerSec << std::endl;
#endif
	} 
    
    //==========================================================================
    StopWatch::StopWatch() throw(Exception, std::bad_alloc)
    //==========================================================================
    : _pImpl(new StopWatchP)
    {
    }

    //--------------------------------------------------------------------------
    StopWatch::~StopWatch() throw()
    //--------------------------------------------------------------------------
    {
        delete _pImpl;
    }
    
    //--------------------------------------------------------------------------
    void StopWatch::start() throw(Exception)
    //--------------------------------------------------------------------------
    {
        if( _pImpl->_isRunning )
		{
			return;
		}
        QueryPerformanceCounter( (LARGE_INTEGER*)&(_pImpl->_startCount));
        _pImpl->_isRunning = true;
#ifdef DEBUG
        std::cout << "[StopWatch::start] " << _pImpl->_startCount << std::endl;
#endif
    }
    
    //--------------------------------------------------------------------------
    void StopWatch::stop() throw(Exception)
    //--------------------------------------------------------------------------
    {
        if( !_pImpl->_isRunning )
        {
            return;
        }

        _pImpl->_isRunning = false;
        QueryPerformanceCounter( (LARGE_INTEGER*)&(_pImpl->_stopCount));
        _pImpl->_accumulatedCount += (_pImpl->_stopCount - _pImpl->_startCount);
#ifdef DEBUG
        std::cout << "[StopWatch::stop] " << _pImpl->_stopCount << std::endl;
#endif
    }
    
    //--------------------------------------------------------------------------
    void StopWatch::reset() throw()
    //--------------------------------------------------------------------------
    {
      _pImpl->zeroAll();
    }

    //--------------------------------------------------------------------------
    long long StopWatch::getTime() const throw(Exception)
    //--------------------------------------------------------------------------
    {
        if( _pImpl->_countsPerSec == 0 )
		{
			return 0;
		}

        if( !_pImpl->_isRunning )
		{
            return static_cast<long long>( (_NANO * _pImpl->_accumulatedCount)/(double)(_pImpl->_countsPerSec) );
		}
		else
		{
			__int64 count;
			QueryPerformanceCounter( (LARGE_INTEGER*)&count);
            return static_cast<long long>( (_NANO * (_pImpl->_accumulatedCount + count - _pImpl->_startCount))/(double)(_pImpl->_countsPerSec) );
		}
    }
    
    //--------------------------------------------------------------------------
    long long StopWatch::getResolution() const throw(Exception)
    //--------------------------------------------------------------------------
    {
        if( _pImpl->_countsPerSec == 0 )
		{
			return 0;
		}
		
        return static_cast<long long>(_NANO/(double)(_pImpl->_countsPerSec));
    }
    
    //--------------------------------------------------------------------------
    bool StopWatch::nanoSleep(long long ns) throw()
    //--------------------------------------------------------------------------
	{
		Sleep( static_cast<DWORD>(ns/1000000LL)); // time specified in ms
		return true;
	}

} // grape


