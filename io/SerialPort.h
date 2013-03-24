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
        BAUD_MAX    //!< not valid as a baud specifier. marks length of enumeration
    };

    /// \brief Data word configuration
    enum DataFormat
    {
        D8N1,   //!< 8 data bits, no parity bit, 1 stop bit
        D7E1,   //!< 7 data bits, even parity, 1 stop bit
        D7O1,   //!< 7 data bits, odd party, 1 stop bit
        DATA_FORMAT_MAX
    };

public:
    SerialPort();
    virtual ~SerialPort();

    /// Set the port name. This should be called before calling open()
    /// \param port Port identifier. Example: /dev/ttyS0 (linux), COM1 (Windows)
    /// \return     true on success. False if a port is already open (call close() first)
    bool setPortName(const std::string& port);

    /// Set baud rate. This works only if the port is already open.
    /// \param baud One of the supported baud rate constants
    /// \return     true on success.
    /// \see open
    bool setBaudRate(BaudRate baud);

    /// Set data format. This works only if the port is already open.
    /// \param fmt One of the supported data format constants
    /// \return     true on success
    /// \see open
    bool setDataFormat(DataFormat fmt);

    /// Enable the use of CTS and RTS signal lines for flow control. This
    /// may not be supported by all platforms. This works only if the port
    /// is already open.
    /// \param enable   Set to true to enable hardware flow control.
    /// \return         true on success.
    bool enableHardwareFlowControl(bool enable);

    /// Enable software flow control using the specified start and stop characters
    /// This works only if the port is already open.
    /// \param enable   Set to true to enable. Remaining parameters are ignored if set to false.
    /// \param xon      Symbol to resume data transmission (default 0x11).
    /// \param xoff     Symbol to pause data transmission (default 0x13)
    /// \return true on success.
    bool enableSoftwareFlowControl(bool enable, char xon = 0x11, char xoff = 0x13);

    /// \copydoc IPort::open()
    /// After open, the serial port must be configured with baud rate, data format, and
    /// flow control options. By default, all flow control is disabled.
    /// A typical configuration is shown below:
    /// \code
    /// SerialPort port;
    /// port.setPortName("/dev/ttyS0"); // a serial port in a unix box
    /// port.open();
    /// port.setBaudRate(B9600);
    /// port.setDataFormat(D8N1);
    /// port.enableHardwareFlowControl(true);
    /// \endcode
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
