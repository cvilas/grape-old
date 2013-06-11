//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IPort.h
// Brief    : Abstract interface for ports
//==============================================================================

#ifndef GRAPEIO_IPORT_H
#define GRAPEIO_IPORT_H

#include "grapeio_common.h"
#include "IoException.h"
#include <vector>

namespace Grape
{

/// \class IPort
/// \ingroup io
/// \brief Abtract interface definition for communication ports
///
/// IPort methods can throw IoException if IO specific exceptions occur
class GRAPEIO_DLL_API IPort
{
public:
    enum Status
    {
        PORT_OK,
        PORT_TIMEOUT,
        PORT_ERROR
    };
public:

    IPort() {}

    /// Open the port.
    /// \note Derived class implements port specific configuration.
    /// \throw IoOpenException
    virtual void open() = 0;

    /// Close the port.
    /// \throw none
    virtual void close() throw(/*nothing*/) = 0;

    /// Check if the port is open
    /// \return true if open
    virtual bool isOpen() = 0;

    /// Read data from the port.
    /// \param buffer   Buffer for read data. If the buffer isn't large enough, it is resized.
    ///                 If the buffer is larger than required, the extra bytes in the buffer are
    ///                 left untouched. Use return value for number of bytes actually read.
    /// \return         The number of bytes read.
    /// \see waitForRead, availableToRead
    /// \throw IoReadException, IoEventHandlingException
    virtual unsigned int read(std::vector<unsigned char>& buffer) = 0;

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
    virtual ~IPort() throw(/*nothing*/) {}
private:
    IPort(const IPort&);            //!< disable copy
    IPort &operator=(const IPort&); //!< disable assignment
};

} // Grape

#endif // GRAPEIO_IPORT_H
