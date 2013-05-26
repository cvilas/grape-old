//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : Timer.h
// Brief    : Hi resolution timer
//==============================================================================

#ifndef GRAPE_TIMER_H
#define	GRAPE_TIMER_H

#include "grapetiming_common.h"
#include "core/Exception.h"

namespace Grape
{
    /// \class Timer
    /// \ingroup timing
    /// \brief Hi resolution interval timer for real-time applications.
    /// 
    /// Usage:
    /// - getResolution() provides the finest time interval t_res (ns) settable with 
    ///   the timer. 
    /// - Call start() with time interval >= t_res. 
    /// - Call wait() or timedWait() to wait for expiry of a clock tick. 
    /// - Implementation is not thread-safe. 

class GRAPETIMING_DLL_API Timer
    {
    public:
        
        /// Create the timer. The timer is unarmed until a call to start().
        explicit Timer() throw(Exception, std::bad_alloc);
        
        /// Destroy the timer.
        ~Timer() throw();
           
        /// Get resolution of the timer in nanoseconds. This is the finest
        /// time interval that can be distinguished by this timer. This should 
        /// be better than 20,000 ns (50Hz) on a POSIX.4 compliant platform.
        /// \return resolution in ns.
        long long getResolution() const throw(Exception);
        
        /// Arm the timer. 
        /// \param ns           (input) Timer tick period in nanoseconds.
        /// \param isOneShot    (input) true if one shot timer, else repeating (default)
        void start(long long ns, bool isOneShot = false) throw(Exception);
        
        /// Disarm the timer.
        void stop() throw(Exception);
        
        /// Wait until a single tick of the timer.
        /// \return true if wait exited due to timer tick.
        bool wait() const throw(Exception);
        
        /// Wait until a single tick of the timer, or until timed out.
        /// \param ns (input) Time out period in nanoseconds. If set to 0, the method
        ///           will return immediately (same as polling for a timer tick).
        /// \return true if wait exited due to timer tick, false on timeout.
        bool timedWait(long long ns) const throw(Exception);
        
        /// Force a timer tick. May use this to unblock wait()
        void forceTimerTick() const throw();
        
        /// Get the number of times the timer has ticked until now.
        long long int getNumTicks() const throw(Exception);
	
	private:
        Timer(const Timer&);                //!< prevent copy
        Timer& operator=(const Timer&);     //!< prevent assignment
        
    private:
        class TimerP* _pImpl;               //!< class private
        
    }; // Timer

} // Grape

#endif	// GRAPE_TIMER_H

