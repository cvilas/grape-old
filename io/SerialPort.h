//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SerialPort.h
// Brief    : Serial port class
//==============================================================================

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "IPort.h"

namespace Grape
{

/// \class SerialPort
/// \ingroup io
/// \brief Serial port communication class
/// \todo
/// - set character size (cs5,6,7,8), stop bits, parity
/// - set hardware, software or no flow control
/// - set canonical or raw mode
/// - other input options (mapping NL, CR, etc)
/// - set read timeout
class GRAPE_DLL_API SerialPort : public IPort
{
public:

    /// \brief Baud rates supported
    enum BaudRate
    {
        B4800 = 0,
        B9600,
        B19200,
        B38400,
        B57600,
        B115200,
        B230400,
        BAUD_MAX //!< not valid as a baud specifier. marks length of enumeration
    };

public:
    SerialPort();
    virtual ~SerialPort();

    /// Set the port name. This should be called before calling open()
    /// \param port port identifier. Example: /dev/ttyS0 (linux), COM1 (Windows)
    /// \return true on success. False if a port is already open (call close() first)
    bool setPortName(const std::string& port);

    /// Set baud rate. This works only if the port is already open.
    /// \param baud One of the supported baud rate constants
    /// \return true on success.
    /// \see open
    bool setBaudRate(BaudRate baud);

    bool open();
    void close();
    bool isOpen();
    int read(std::vector<char>& buffer);
    int write(const std::vector<char>& buffer);
    bool waitForRead(int timeoutMs);
    bool waitForWrite(int timeoutMs);

private:
    class SerialPortP* _pImpl; //!< platform specific private implementation

}; // SerialPort


} // Grape

#endif // SERIALPORT_H
