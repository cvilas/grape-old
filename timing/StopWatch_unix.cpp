//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : StopWatch_unix.cpp
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
#include "core/posix.h"
#include <errno.h>
#ifdef DEBUG
#       include <iostream>
#endif
namespace grape
{
    /// posix specific implementation class
    
    class StopWatchP
    {
    public:
        StopWatchP() throw() { zeroAll(); }
        ~StopWatchP() throw() {}
        void zeroAll() throw();
        
    private:
        StopWatchP(const StopWatchP&);                //!< prevent copy construction
        StopWatchP& operator=(const StopWatchP&);     //!< prevent assignment

    public:
        struct timespec _startTime;
        struct timespec _stopTime;
        struct timespec _accumulatedTime;
        bool _isRunning;
        static const clockid_t _CLOCKID = DEFAULT_CLOCK;

    }; // StopWatchP

    //==========================================================================
    void StopWatchP::zeroAll() throw()
    //==========================================================================
    {
        _isRunning = false;
        _startTime.tv_nsec = 0;
        _startTime.tv_sec = 0;
        _stopTime.tv_nsec = 0;
        _stopTime.tv_sec = 0;
        _accumulatedTime.tv_nsec = 0;
        _accumulatedTime.tv_sec = 0;
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
        
        if( clock_gettime(StopWatchP::_CLOCKID, &_pImpl->_startTime) < 0 )
        {
            throw Exception(errno, "[StopWatch::start]");
        }
        _pImpl->_isRunning = true;
#ifdef DEBUG
        std::cout << "[StopWatch::start] " << _pImpl->_startTime.tv_sec << ", " << _pImpl->_startTime.tv_nsec << std::endl;
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

        if( clock_gettime(StopWatchP::_CLOCKID, &_pImpl->_stopTime) < 0 )
        {
            throw Exception(errno, "[StopWatch::stop]");
        }
        _pImpl->_isRunning = false;
        
        struct timespec elapsed;
        if( !timespecDiff(_pImpl->_stopTime, _pImpl->_startTime, elapsed) )
        {
            throw Exception(-1, "[StopWatch::stop] stop time < start time");
        }
        timespecAdd(_pImpl->_accumulatedTime, elapsed, _pImpl->_accumulatedTime);
#ifdef DEBUG
        std::cout << "[StopWatch::stop] " << _pImpl->_stopTime.tv_sec << ", " << _pImpl->_stopTime.tv_nsec << std::endl;
#endif
    }
    
    //--------------------------------------------------------------------------
    void StopWatch::reset() throw()
    //--------------------------------------------------------------------------
    {
        _pImpl->zeroAll();
    }

    //--------------------------------------------------------------------------
    long long StopWatch::getAccumulatedNanoseconds() const throw(Exception)
    //--------------------------------------------------------------------------
    {
        if( _pImpl->_isRunning )
        {
            struct timespec now;
            if(  clock_gettime(StopWatchP::_CLOCKID, &now) < 0 )
            {
                throw Exception(errno, "[StopWatch::getTime]");
            }
            struct timespec elapsed;
            if( !timespecDiff(now, _pImpl->_startTime, elapsed) )
            {
                throw Exception(-1, "[StopWatch::getTime] current time < start time");
            }
            timespecAdd(_pImpl->_accumulatedTime, elapsed, elapsed);
            return elapsed.tv_sec * _NANO + elapsed.tv_nsec;
        }
        else
        {
            return _pImpl->_accumulatedTime.tv_sec * _NANO + _pImpl->_accumulatedTime.tv_nsec;
        }
    }
    
    //--------------------------------------------------------------------------
    long long StopWatch::getResolutionNanoseconds() const throw(Exception)
    //--------------------------------------------------------------------------
    {
        struct timespec res;
        res.tv_sec = 0;
        res.tv_nsec = 0;
        if( clock_getres(StopWatchP::_CLOCKID, &res) < 0 )
        {
            throw Exception(errno, "[StopWatch::getResolution]");
        }
        return (res.tv_sec * _NANO) + res.tv_nsec;
    }
    
    //--------------------------------------------------------------------------
    bool StopWatch::nanoSleep(long long ns) throw()
    //--------------------------------------------------------------------------
    {
        struct timespec t;
        t.tv_sec = (long)(ns/_NANO);
        t.tv_nsec = (long)(ns%_NANO);
        return ( 0 == nanosleep(&t, NULL) );
    }

} // grape


