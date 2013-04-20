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
    /// Notes for POSIX platforms:
    /// - Only TIMER_MAX (defined in limits.h) number of timers can 
    ///   be created at any time. Therefore, it makes sense to delete active 
    ///   timers when you don't need them anymore. 
    /// - The Timer internally relies on POSIX.4 real-time extensions to 
    ///   signals, and uses signals between SIGRTMIN and SIGRTMAX (both inclusive)
    ///   to notify timer ticks. To guarantee that an object of this
    ///   class is always notified of timer expiry, the specific RT signal that 
    ///   is used internally is blocked from delivery to any other signal handler
    ///   in the process. 
    /// 
    /// Notes for Windows platforms:
    /// - Class uses multimedia timers internally.
    /// - Timer priority (constructor parameter) is ignored.
    ///
    /// Usage:
    /// - getResolution() provides the finest time interval t_res (ns) settable with 
    ///   the timer. 
    /// - Call start() with time interval >= t_res. 
    /// - Call wait() or timedWait() to wait for expiry of a clock tick. 
    /// - getNotificationSignal() will tell you what specific signal we are using.
    ///   Don't use this signal elsewhere.
    /// - Implementation is not thread-safe. 
    ///
    /// <b>Example Program:</b>
    /// \include TimerExample.cpp

    class GRAPETIMING_DLL_API Timer
    {
    public:
        
        /// Create the timer. The timer is unarmed until a call to start().
        /// Note: The timer internally uses the POSIX realtime signals SIGRTMIN
        /// to SIGRTMAX. The default action for a thread receiving an RT signal
        /// is to terminate itself. Therefore, CTimer class masks the RT signal
        /// it uses for the thread where it is created. In a multi-threaded
        /// application, the user must explicitly mask this signal in all other
        /// threads to avoid getting terminated. See getNotificationSignal().
        /// \param prio (input) (Valid only for POSIX) A non-negative value indicating priority
        ///             of this timer. Set this to a higher value for higher 
        ///             priority and higher frequency timers. 
        ///             Internally the timer uses SIGRTMAX (lowest priority)
        ///             to SIGRTMIN (highest priority) to signal timer ticks. (See
        ///             getNotifierSignal(). POSIX.4 compliant systems guarantee 
        ///             at least 8 signals between SIGRTMIN and SIGRTMAX.
        ///		This parameter is ignored in Windows.
        explicit Timer(int prio = 0) throw(Exception, std::bad_alloc);
        
        /// Destroy the timer.
        ~Timer() throw();
        
        /// \return POSIX: The timer expiry signal used internally, between SIGRTMIN
        ///         and SIGRTMAX.
        ///         Linux: Use this information to block this signal from being
        ///         received by other threads in the system. Otherwise, the default
        ///         action for RT signal is to terminate the thread. To block
        ///         a signal in a thread, use pthread_sigmask as follows:
        ///         \code
        ///         int sig = getNotificationSignal();
        ///         sigset_t set, oset;
        ///         sigemptyset(&set);
        ///         sigaddset(&set, sig);
        ///         pthread_sigmask(SIG_BLOCK, &set, &oset);
        ///         \endcode
        ///         Windows: -1. Not implemented.
        int getNotificationSignal() const throw();
        
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
        /// \return true on success, false on error. (Most likely interrupted by a signal handler)
        bool wait() const throw();
        
        /// Wait until a single tick of the timer, or until timed out.
        /// \param ns (input) Time out period in nanoseconds. If set to 0, the method
        ///           will return immediately (same as polling for a timer tick).
        /// \return true on success, false on error.
        bool timedWait(long long ns) const throw();
        
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

