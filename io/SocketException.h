//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SocketException.h
// Brief    : Socket exceptions
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

#ifndef GRAPE_SOCKETEXCEPTION_H
#define GRAPE_SOCKETEXCEPTION_H

#include "grapeio_common.h"
#include "IoException.h"

namespace grape
{

/// \brief common base class for exceptions thrown by socket classes
/// \ingroup io
class GRAPEIO_DLL_API SocketException : public grape::IoException
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


} // grape

#endif // GRAPE_SOCKETEXCEPTION_H
