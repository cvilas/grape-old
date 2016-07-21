//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SerialPortException.h
// Brief    : Exceptions thrown by serial port
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

#ifndef GRAPE_SERIALPORTEXCEPTION_H
#define GRAPE_SERIALPORTEXCEPTION_H

#include "grapeio_common.h"
#include "IoException.h"

namespace grape
{

/// \brief common base class for exceptions thrown by serial port classes
/// \ingroup io
class GRAPEIO_DLL_API SerialPortException : public grape::IoException
{
public:
    SerialPortException() throw() : IoException(0, "Unspecified SerialPort Exception") {}
    SerialPortException(int code, const std::string& msg) throw() : IoException(code, msg.c_str()) {}
    SerialPortException(const SerialPortException& ve) throw() : IoException(ve) {}
    SerialPortException& operator=(const SerialPortException& other) throw() { IoException::operator=(other); return *this; }
}; // SerialPortException

/// \brief Invalid/unsupported baud rate
/// \ingroup io
class GRAPEIO_DLL_API InvalidBaudException : public grape::SerialPortException
{
public:
    InvalidBaudException() throw() : SerialPortException(0, "Invalid/unsupported baud rate") {}
    InvalidBaudException(int code, const std::string& msg) throw() : SerialPortException(code, msg.c_str()) {}
    InvalidBaudException(const InvalidBaudException& ve) throw() : SerialPortException(ve) {}
    InvalidBaudException& operator=(const InvalidBaudException& other) throw() { SerialPortException::operator=(other); return *this; }
}; // SerialPortException

/// \brief Invalid/unsupported serial data format
/// \ingroup io
class GRAPEIO_DLL_API InvalidSerialDataFormatException : public grape::SerialPortException
{
public:
    InvalidSerialDataFormatException() throw() : SerialPortException(0, "Invalid/unsupported baud rate") {}
    InvalidSerialDataFormatException(int code, const std::string& msg) throw() : SerialPortException(code, msg.c_str()) {}
    InvalidSerialDataFormatException(const InvalidBaudException& ve) throw() : SerialPortException(ve) {}
    InvalidSerialDataFormatException& operator=(const InvalidBaudException& other) throw() { SerialPortException::operator=(other); return *this; }
}; // SerialPortException

} // grape

#endif // GRAPE_SERIALPORTEXCEPTION_H
