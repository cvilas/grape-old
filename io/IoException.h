//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IoException.h
// Brief    : Exceptions raised by Grape IO
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

#ifndef GRAPE_IOEXCEPTION_H
#define GRAPE_IOEXCEPTION_H

#include "grapeio_common.h"
#include "core/Exception.h"

namespace grape
{

/// \brief common base class for exceptions thrown by IO classes
/// \ingroup io
class GRAPEIO_DLL_API IoException : public grape::Exception
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

} // grape

#endif // GRAPE_IOEXCEPTION_H
