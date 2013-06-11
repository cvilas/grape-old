//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IoException.h
// Brief    : Exceptions raised by Grape IO
//==============================================================================

#ifndef GRAPE_IOEXCEPTION_H
#define GRAPE_IOEXCEPTION_H

#include "grapeio_common.h"
#include "core/Exception.h"

namespace Grape
{

/// \brief common base class for exceptions thrown by IO classes
/// \ingroup io
class GRAPEIO_DLL_API IoException : public Grape::Exception
{
public:
    IoException() throw() : Exception(0, "Unspecified IO Exception") {}
    IoException(int code, const std::string& msg) throw() : Exception(code, msg.c_str()) {}
    IoException(const IoException& ve) throw() : Exception(ve) {}
    IoException& operator=(const IoException& other) throw() { Exception::operator=(other); return *this; }
}; // IoException

/// \brief Unable to open a communication IO channel
/// \ingroup io
class GRAPEIO_DLL_API IoOpenException : public IoException
{
public:
    IoOpenException() throw() : IoException(0, "Unable to open IO port") {}
    IoOpenException(int code, const std::string& msg) throw() : IoException(code, msg.c_str()) {}
    IoOpenException(const IoOpenException& ve) throw() : IoException(ve) {}
    IoOpenException& operator=(const IoOpenException& other) throw() { IoException::operator=(other); return *this; }
}; // IoOpenException

/// \brief Unable to read communication IO channel
/// \ingroup io
class GRAPEIO_DLL_API IoReadException : public IoException
{
public:
    IoReadException() throw() : IoException(0, "Unable to read IO") {}
    IoReadException(int code, const std::string& msg) throw() : IoException(code, msg.c_str()) {}
    IoReadException(const IoReadException& ve) throw() : IoException(ve) {}
    IoReadException& operator=(const IoReadException& other) throw() { IoException::operator=(other); return *this; }
}; // IoReadException

/// \brief Unable to write to communication IO channel
/// \ingroup io
class GRAPEIO_DLL_API IoWriteException : public IoException
{
public:
    IoWriteException() throw() : IoException(0, "Unable to read IO") {}
    IoWriteException(int code, const std::string& msg) throw() : IoException(code, msg.c_str()) {}
    IoWriteException(const IoWriteException& ve) throw() : IoException(ve) {}
    IoWriteException& operator=(const IoWriteException& other) throw() { IoException::operator=(other); return *this; }
}; // IoWriteException

/// \brief Error occurred in IO event handling mechanism
/// \ingroup io
class GRAPEIO_DLL_API IoEventHandlingException : public IoException
{
public:
    IoEventHandlingException() throw() : IoException(0, "Unable to read IO") {}
    IoEventHandlingException(int code, const std::string& msg) throw() : IoException(code, msg.c_str()) {}
    IoEventHandlingException(const IoEventHandlingException& ve) throw() : IoException(ve) {}
    IoEventHandlingException& operator=(const IoEventHandlingException& other) throw() { IoException::operator=(other); return *this; }
}; // IoEventException

} // Grape

#endif // GRAPE_IOEXCEPTION_H
