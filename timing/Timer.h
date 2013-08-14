//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : Timer.h
// Brief    : Hi resolution timer
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

