//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : StopWatch.h
// Brief    : Hi resolution stop watch  
//==============================================================================

#ifndef GRAPE_STOPWATCH_H
#define	GRAPE_STOPWATCH_H

#include "grapetiming_common.h"
#include "core/Exception.h"

namespace Grape 
{
    
    /// \class StopWatch
    /// \ingroup timing
    /// \brief Hi resolution stop watch for real-time applications
    /// 
    /// Notes
    /// - Implementation is not thread-safe.
    /// - In Windows, the class uses QueryPerformanceCounter(QPC) for obtaining clock ticks,
    ///   and QueryPerformanceFrequency (QPF) for obtaining clock rate. Calling QPC 
    ///   itself takes about 5 microseconds to execute on a 2 GHz processor.
    ///   In multiprocessor environments, subsequent calls to QueryPerformanceCounter 
    ///   may be handled by different processors or cores and time differences reported 
    ///   may be wrong (even negative) because the processors are not in sync. This is 
    ///   not supposed to happen according to MS SDK, but may happen due to buggy BIOS or 
    ///   HAL (harware abstraction layer). For a detailed note on timing issues, see 
    ///   http://msdn.microsoft.com/en-us/library/bb173458.aspx
    ///
    /// <b>Example Program:</b>
    /// \include StopWatchExample.cpp

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
        long long getTime() const throw(Exception);
        
        /// \return Clock resolution in nano-seconds. 
        long long getResolution() const throw(Exception);

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

} // Grape

#endif	// GRAPE_STOPWATCH_H


