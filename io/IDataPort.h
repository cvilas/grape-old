//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IDataPort.h
// Brief    : Abstract interface for data streams
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

#ifndef GRAPEIO_IDATAPORT_H
#define GRAPEIO_IDATAPORT_H

#include "grapeio_common.h"
#include "IoException.h"
#include <vector>

namespace grape
{

/// \class IDataPort
/// \ingroup io
/// \brief Abtract interface definition for communication ports
///
/// IDataPort methods can throw IoException if IO specific exceptions occur
class GRAPEIO_DLL_API IDataPort
{
public:
    enum Status
    {
        PORT_OK,
        PORT_TIMEOUT,
        PORT_ERROR
    };
public:

    virtual ~IDataPort() throw(/*nothing*/) {}

    /// Close the port.
    /// \throw none
    virtual void close() throw(/*nothing*/) = 0;

    /// Read all available data from the port.
    /// \param buffer   Buffer for read data. If the buffer isn't large enough, it is resized.
    ///                 If the buffer is larger than required, the extra bytes in the buffer are
    ///                 left untouched. Use return value for number of bytes actually read.
    /// \return         The number of bytes read.
    /// \see waitForRead, availableToRead
    /// \throw IoReadException, IoEventHandlingException
    virtual unsigned int readAll(std::vector<unsigned char>& buffer) = 0;

    /// Read specified number of data bytes from the port.
    /// \param buffer   Buffer for read data. If the buffer isn't large enough, it is resized.
    ///                 If the buffer is larger than required, the extra bytes in the buffer are
    ///                 left untouched. Use return value for number of bytes actually read.
    /// \param bytes    Number of bytes to read
    /// \return         The number of bytes read.
    /// \see waitForRead, availableToRead
    /// \throw IoReadException, IoEventHandlingException
    virtual unsigned int readn(std::vector<unsigned char>& buffer, unsigned int bytes) = 0;

    /// Find the number of bytes available and waiting to be read
    /// without actually reading them.
    /// \return The number of bytes available to read.
    /// \see waitForRead, read
    virtual unsigned int availableToRead() = 0;

    /// Wait until port is ready for a read operation
    /// \param timeoutMs    Milliseconds to wait before returning.
    ///                     Set negative number for infinite wait period.
    /// \return status code
    /// \see read, availableToRead
    /// \throw IoEventHandlingException
    virtual Status waitForRead(int timeoutMs) = 0;

    /// Flush data received but not read
    virtual void flushRx() = 0;

    /// Write data to the port
    /// \param buffer   Buffer containing data to be written. Note that not all data
    ///                 may get written. Return value for number of bytes actually written.
    /// \return         Number of bytes written.
    /// \see waitForWrite
    /// \throw IoWriteException, IoEventHandlingException
    virtual unsigned int write(const std::vector<unsigned char>& buffer) = 0;

    /// Wait until all bytes from last write operation have been transmitted
    /// \param timeoutMs    Milliseconds to wait before returning.
    ///                     Specify negative number for infinite wait period.
    /// \return status code
    /// \see write
    virtual Status waitForWrite(int timeoutMs) = 0;


    /// Flush data written but not transmitted
    virtual void flushTx() = 0;

protected:
    IDataPort() {}

}; // IDataPort

} // grape

#endif // GRAPEIO_IDATAPORT_H
