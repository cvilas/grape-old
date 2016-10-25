//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : StopWatch.h
// Brief    : Hi resolution stop watch  
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

#ifndef GRAPE_STOPWATCH_H
#define	GRAPE_STOPWATCH_H

#include "grapetiming_common.h"
#include "core/Exception.h"

namespace grape 
{
    
    /// \class StopWatch
    /// \ingroup timing
    /// \brief Hi resolution stop watch for real-time applications
    /// 
    /// \note Resolution is platform dependant.
    /// \note Implementation is not thread-safe.
    /// \note In Windows, the class uses QueryPerformanceCounter(QPC) for obtaining clock ticks,
    /// and QueryPerformanceFrequency (QPF) for obtaining clock rate. Calling QPC
    /// itself takes about 5 microseconds to execute on a 2 GHz processor.
    /// In multiprocessor environments, subsequent calls to QueryPerformanceCounter
    /// may be handled by different processors or cores and time differences reported
    /// may be wrong (even negative) because the processors are not in sync. This is
    /// not supposed to happen according to MS SDK, but may happen due to buggy BIOS or
    /// HAL (harware abstraction layer). For a detailed note on timing issues, see
    /// http://msdn.microsoft.com/en-us/library/bb173458.aspx

    class GRAPETIMING_DLL_API StopWatch
    {
    public:
        StopWatch() throw(Exception, std::bad_alloc);

        ~StopWatch() throw();

        /// Start counting time. 
        void start() throw(Exception);

        /// Stop counting time. On calling start() again, the counter continues
        /// from where it stopped, rather than starting from 0. 
        void stop() throw(Exception);

        /// Reset watch. On calling start() again, the time starts from 0.
        void reset() throw();

        /// \return The accumulated time interval in nano-seconds between multiple start-stop
        /// calls since the the last call to reset().
        /// \note Time measurement within each start-stop is only as good as getResolution()
        long long getAccumulatedNanoseconds() const throw(Exception);
        
        /// \return Clock resolution in nano-seconds. 
        long long getResolutionNanoseconds() const throw(Exception);

        /// Sleep for specified time in nano-seconds.
        /// Note: Sleep time resolution is only as good as getResolution()
        /// \return true on successfully sleeping for the specified interval.
        static bool nanoSleep(long long ns) throw();

    private:
        StopWatch(const StopWatch&);                //!< prevent copy construction
        StopWatch& operator=(const StopWatch&);     //!< prevent assignment

    private:
        class StopWatchP*       _pImpl;	//!< implementation private
        static const long long  _NANO = 1000000000LL;

    }; // StopWatch

} // grape

#endif	// GRAPE_STOPWATCH_H


