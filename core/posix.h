//==============================================================================
// Project  : Grape
// Module   : core
// File     : posix.h
// Brief    : Standard header for POSIX compliance and support
//==============================================================================

/// \todo
/// - harness for runtime checks using sysconf, pathconf and fpathconf (POSIX
///   support at compile time may differ from that available at runtime)

#ifndef GRAPE_POSIX_H
#define	GRAPE_POSIX_H

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE   1       // old style feature selector
#endif

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L // use 9/93 POSIX.1, .2 and .4 definitions only
#endif

#include <unistd.h>             // for POSIX definitions

#ifndef _POSIX_TIMERS
#error "POSIX timer support not available."
#endif

#ifndef _POSIX_REALTIME_SIGNALS
#error "POSIX real-time signals support not available."
#endif

#ifndef _POSIX_PRIORITY_SCHEDULING
#error "POSIX real-time scheduling support not available."
#endif


#include <signal.h>
#include <time.h>
#include <sched.h>
#include <inttypes.h>

#define DEFAULT_CLOCK CLOCK_REALTIME // only clock guaranteed to be available

namespace grape
{
    /// Subtract one timespec data from another and return the difference
    /// \return false if end time is before start time
    inline bool timespecDiff(const struct timespec end, const struct timespec start, struct timespec& diff) throw();
    
    // Add two timespec data together and return sum
    inline void timespecAdd(const struct timespec& t0, const struct timespec& t1, struct timespec& sum) throw();

    //--------------------------------------------------------------------------
    bool timespecDiff (struct timespec end, struct timespec start, struct timespec& diff) throw()
    //--------------------------------------------------------------------------
    {
        static long long nano = 1000000000LL;
        
        // from http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
        // Perform the carry for the later subtraction by updating y.
        if (end.tv_nsec < start.tv_nsec) 
        {
            int nsec = (start.tv_nsec - end.tv_nsec) / nano + 1;
            start.tv_nsec -= nano * nsec;
            start.tv_sec += nsec;
        }
        if (end.tv_nsec - start.tv_nsec > nano) 
        {
            int nsec = (end.tv_nsec - start.tv_nsec) / nano;
            start.tv_nsec += nano * nsec;
            start.tv_sec -= nsec;
        }
         
        // Compute the time remaining to wait. tv_usec is certainly positive. 
        diff.tv_sec = end.tv_sec - start.tv_sec;
        diff.tv_nsec = end.tv_nsec - start.tv_nsec;
        
        // Return false if result is negative. 
        return ( end.tv_sec < start.tv_sec ) ? (false) : (true);
    }          
          
    //--------------------------------------------------------------------------
    void timespecAdd(const struct timespec& t0, const struct timespec& t1, struct timespec& sum) throw()
    //--------------------------------------------------------------------------
    {
        const long long nano = 1000000000LL;
        
        sum.tv_sec = t0.tv_sec + t1.tv_sec;
        sum.tv_nsec = t0.tv_nsec + t1.tv_nsec;
        
        long long nsec = sum.tv_nsec / nano;
        sum.tv_nsec -= nano * nsec;
        sum.tv_sec += nsec;
    }

} // Grape

#endif	// GRAPE_POSIX_H

