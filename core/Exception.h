//==============================================================================
// Project  : Grape
// Module   : Core
// File     : Exception.h
// Brief    : Exception class
//==============================================================================

#ifndef GRAPE_EXCEPTION_H
#define	GRAPE_EXCEPTION_H

#include "grapecore_common.h"
#include <stdexcept>
#include <string.h>

namespace Grape
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
    
} // Grape

#endif	// GRAPE_EXCEPTION_H
