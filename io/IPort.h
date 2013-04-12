//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IPort.h
// Brief    : Abstract interface for ports
//==============================================================================

#ifndef GRAPEIO_IPORT_H
#define GRAPEIO_IPORT_H

#include "grapeio_common.h"
#include "core/Status.h"
#include <vector>

namespace Grape
{

/// \class IPort
/// \ingroup io
/// \brief Abtract interface definition for communication ports
class GRAPEIO_DLL_API IPort
{
public:
    /// Publically accessible status object contains the last
    /// error code and message
    /// \note Derived classes must call Status::set on unsuccessful operations.
    Status lastError;

public:

    IPort() {}

    /// Open the port.
    /// \note Derived class implements port specific configuration.
    /// \return true on success
    /// \see getLastError
    virtual bool open() = 0;

    /// Close the port.
    virtual void close() = 0;

    /// Check if the port is open
    /// \return true if open
    virtual bool isOpen() = 0;

    /// Read data from the port.
    /// \param buffer   Contains read data. Note that this buffer may be longer than the number of
    ///                 bytes read. Use return value for number of bytes actually read.
    /// \return         The number of bytes read. -1 on error.
    /// \see waitForRead, getLastError
    virtual int read(std::vector<unsigned char>& buffer) = 0;

    /// Find the number of bytes available and waiting to be read
    /// without actually reading them.
    /// \return The number of bytes available to read. -1 on error.
    /// \see getLastError
    virtual int availableToRead() = 0;

    /// Write data to the port
    /// \param buffer   Contains data to be written. Note that not all data may get written.
    ///                 Use return value for number of bytes actually written.
    /// \return         Number of bytes written. -1 on error.
    /// \see waitForWrite, getLastError
    virtual int write(const std::vector<unsigned char>& buffer) = 0;

    /// Wait until port is ready for a read operation
    /// \param timeoutMs    Milliseconds to wait before returning.
    ///                     Set negative number for infinite wait period.
    /// \return >0 if port is ready for read, 0 on timeout, <0 on error.
    /// \see read, getLastError
    virtual int waitForRead(int timeoutMs) = 0;

    /// Wait until all bytes from last write operation have been transmitted
    /// \param timeoutMs    Milliseconds to wait before returning.
    ///                     Specify negative number for infinite wait period.
    /// \return >0 if last write operation finished, 0 on timeout, <0 on error.
    /// \see write, getLastError
    virtual int waitForWrite(int timeoutMs) = 0;

    /// Flush data received but not read
    virtual void flushRx() = 0;

    /// Flush data written but not transmitted
    virtual void flushTx() = 0;

protected:
    virtual ~IPort() {}
private:
    IPort(const IPort&);            //!< disable copy
    IPort &operator=(const IPort&); //!< disable assignment
};

} // Grape

#endif // GRAPEIO_IPORT_H
