//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IPort.h
// Brief    : Abstract interface for ports
//==============================================================================

#ifndef IPORT_H
#define IPORT_H

#include "grape_common.h"
#include <vector>
#include <sstream>
#include <string>

namespace Grape
{

/// \class IPort
/// \ingroup io
/// \brief Abtract interface definition for communication ports
class IPort
{
public:

    IPort() : _errorCode(0) {}

    /// Open the port.
    /// Note: Derived class implements port specific configuration.
    /// \return true on success
    /// \see getLastError
    virtual bool open() = 0;

    /// Close the port.
    virtual void close() = 0;

    /// Check if the port is open
    /// \return true if open
    virtual bool isOpen() = 0;

    /// Read data from the port.
    /// \return The number of bytes read. -1 on error.
    /// \see waitForRead, getLastError
    virtual int read(std::vector<char>& buffer) = 0;

    /// Write data to the port
    /// \return Number of bytes written. -1 on error.
    /// \see waitForWrite, getLastError
    virtual int write(const std::vector<char>& buffer) = 0;

    /// Wait until port is ready for a read operation
    /// \param timeoutMs Milliseconds to wait before returning.
    ///                  Set negative number for infinite wait period.
    /// \return true if port is ready for read, false on timeout or error.
    /// \see read, getLastError
    virtual bool waitForRead(int timeoutMs) = 0;

    /// Wait until port is ready for a write operation
    /// \param timeoutMs Milliseconds to wait before returning.
    ///                  Specify negative number for infinite wait period.
    /// \return true if port is ready for write, false on time out or error
    /// \see write, getLastError
    virtual bool waitForWrite(int timeoutMs) = 0;

    /// Get the last error description.
    /// \param errorCode Contains code number for the error, if any
    /// \return nothing if no errors occurred since last error
    std::string getLastError(int& errorCode);

protected:
    virtual ~IPort() {}
    void clearLastError();

private:
    IPort(const IPort&); //!< disable copy
    IPort &operator=(const IPort&); //!< disable assignment

protected:
    std::ostringstream  _errorString;
    int                 _errorCode;
};

//-----------------------------------------------------------------------------
std::string IPort::getLastError(int& errorCode)
//-----------------------------------------------------------------------------
{
    errorCode = _errorCode;
    std::string str = _errorString.str();
    clearLastError();
    return str;
}

//-----------------------------------------------------------------------------
void IPort::clearLastError()
//-----------------------------------------------------------------------------
{
    _errorCode = 0;
    _errorString.clear();
}

} // Grape

#endif // IPORT_H
