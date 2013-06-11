//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IDataPort.h
// Brief    : Abstract interface for data streams
//==============================================================================

#ifndef GRAPEIO_IDATAPORT_H
#define GRAPEIO_IDATAPORT_H

#include "grapeio_common.h"
#include "IoException.h"
#include <vector>

namespace Grape
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

    IDataPort() {}

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
    virtual ~IDataPort() throw(/*nothing*/) {}
private:
    IDataPort(const IDataPort&);            //!< disable copy
    IDataPort &operator=(const IDataPort&); //!< disable assignment
}; // IDataPort

} // Grape

#endif // GRAPEIO_IDATAPORT_H
