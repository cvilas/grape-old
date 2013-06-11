//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SocketException.h
// Brief    : Socket exceptions
//==============================================================================

#ifndef GRAPE_SOCKETEXCEPTION_H
#define GRAPE_SOCKETEXCEPTION_H

#include "grapeio_common.h"
#include "IoException.h"

namespace Grape
{

/// \brief common base class for exceptions thrown by socket classes
/// \ingroup io
class GRAPEIO_DLL_API SocketException : public Grape::IoException
{
public:
    SocketException() throw() : IoException(0, "Unspecified Socket Exception") {}
    SocketException(int code, const std::string& msg) throw() : IoException(code, msg.c_str()) {}
    SocketException(const SocketException& ve) throw() : IoException(ve) {}
    SocketException& operator=(const SocketException& other) throw() { IoException::operator=(other); return *this; }
}; // ControllerException


/// \brief Windows socket initialisation error
/// \ingroup io
class GRAPEIO_DLL_API WsaInitException : public SocketException
{
public:
    WsaInitException() throw() : SocketException(0, "Unspecified WSA Exception") {}
    WsaInitException(int code, const std::string& msg) throw() : SocketException(code, msg.c_str()) {}
    WsaInitException(const WsaInitException& ve) throw() : SocketException(ve) {}
    WsaInitException& operator=(const WsaInitException& other) throw() { SocketException::operator=(other); return *this; }

}; // WsaInitException

/// \brief Could not get information about host
/// \ingroup io
class GRAPEIO_DLL_API HostInfoException : public SocketException
{
public:
    HostInfoException() throw() : SocketException(0, "Unspecified host resolution Exception") {}
    HostInfoException(int code, const std::string& msg) throw() : SocketException(code, msg.c_str()) {}
    HostInfoException(const HostInfoException& ve) throw() : SocketException(ve) {}
    HostInfoException& operator=(const HostInfoException& other) throw() { SocketException::operator=(other); return *this; }

}; // HostnameException


} // Grape

#endif // GRAPE_SOCKETEXCEPTION_H
