//==============================================================================
// Project  : Grape
// Module   : Core
// File     : Exception.h
// Brief    : Exception class
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

#ifndef GRAPE_EXCEPTION_H
#define	GRAPE_EXCEPTION_H

#include "grapecore_common.h"
#include <stdexcept>
#include <string.h>

namespace grape
{

    /// \class Exception
    /// \ingroup core
    /// \brief Basic exception object
    ///
    /// - Exception provides a means to throw an exception with an error code
    ///   (example: errno) and a short message.
    /// - Methods of the class itself will not throw exceptions.
    /// - Design follows Boost's exception class design guidelines:
    ///   www.boost.org/community/error_handling.html
    /// - For older versions of gcc ( <= 2.96 ) compile with exceptions enabled 
    ///   (-fexceptions switch) to avoid SIGABRT on exception.

    class GRAPECORE_DLL_API Exception : public std::exception
    {
    public:
        static const size_t MAX_MESSAGE_LENGTH = 256; //!< max length of message
        
    public:

        /// Standard constructor
        /// \param code  set integer error code.
        /// \param desc  set a short description, possibly just the cause and location of the
        ///              error.
        explicit Exception(int code, const char* desc="(no description)") throw(/*nothing*/);
        
        /// copy constructor
        Exception(const Exception& e) throw(/*nothing*/);
        
        /// destructor
        virtual ~Exception() throw(/*nothing*/) { }

        /// assignment
        Exception& operator=(const Exception& e) throw(/*nothing*/);
        
        /// \return A description of the exception thrown
        virtual const char* what() const throw(/*nothing*/) { return _msgStr; }
        
        /// \return latest error code.
        int code() const throw(/*nothing*/) { return _code; }

    private:
        void copyFrom(int code, const char* msg) throw(/*nothing*/);
        int _code;
        char _msgStr[MAX_MESSAGE_LENGTH];

    }; // Exception
    
} // grape

#endif	// GRAPE_EXCEPTION_H
