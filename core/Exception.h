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
    /// - CException provides a means to throw an exception with an error code
    ///   (example: errno) and a short message.
    /// - Methods of the class itself will not throw exceptions.
    /// - Design follows Boost's exception class design guidelines:
    ///   www.boost.org/community/error_handling.html
    /// - For older versions of gcc ( <= 2.96 ) compile with exceptions enabled 
    ///   (-fexceptions switch) to avoid SIGABRT on exception.
    /// - Define GENERICLIB_DLL in user code if linking against library dll in Windows
    /// 
    /// <b>Example Program:</b>
    /// \include ExceptionExample.cpp

    class GRAPECORE_DLL_API Exception : public std::exception
    {
    public:
        static const size_t MAX_MSG_LEN = 80; //!< max length of message
        
    public:

        /// Standard constructor
        /// \param code  set integer error code. (0 reserved for no error)
        /// \param desc  set a short description, possibly just the location of the
        ///              error.
        explicit Exception(int code, const char* desc="(no description)") throw(/*nothing*/);
        
        /// copy constructor
        Exception(const Exception& e);
        
        /// destructor
        virtual ~Exception() throw(/*nothing*/) { }

        /// assignment
        Exception& operator=(const Exception& e) throw(/*nothing*/);
        
        /// \return A description of the exception thrown
        virtual const char* what() const throw(/*nothing*/) { return _msgStr; }
        
        /// \return latest error code. (0 means no error).
        int getCode() const throw(/*nothing*/) { return _code; }

    private:
        void copyFrom(int code, const char* msg) throw(/*nothing*/);
        int _code;
        char _msgStr[MAX_MSG_LEN];

    }; // Exception
    
} // Grape

#endif	// GRAPE_EXCEPTION_H
