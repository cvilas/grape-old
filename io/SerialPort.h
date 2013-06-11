//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SerialPort.h
// Brief    : Serial port class
//==============================================================================

#ifndef GRAPEIO_SERIALPORT_H
#define GRAPEIO_SERIALPORT_H

#include "IPort.h"
#include "SerialPortException.h"

namespace Grape
{

/// \class SerialPort
/// \ingroup io
/// \brief A simple serial port communication class
/// The class provides a basic interface to serial ports.
/// \todo
/// - Test all bytes have been transmitted in waitForWrite()
/// - In windows, review overlapped io timeouts are correct
class GRAPEIO_DLL_API SerialPort : public IPort
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
    virtual ~SerialPort() throw (/*nothing*/);

    /// Set the port name. This should be called before calling open()
    /// \param port Port identifier. Example: /dev/ttyS0 (linux), COM1 (Windows)
    void setPortName(const std::string& port);
    std::string getPortName() const;

    /// Set baud rate. This works only if the port is already open.
    /// \param baud One of the supported baud rate constants
    /// \see open
    /// \throw InvalidBaudException
    void setBaudRate(BaudRate baud);

    /// Set data format. This works only if the port is already open.
    /// \param fmt One of the supported data format constants
    /// \see open
    /// \throw InvalidSerialDataFormatException
    void setDataFormat(DataFormat fmt);

    /// \copydoc IPort::open()
    /// After open, the serial port must be configured with baud rate and data format.
    /// A typical configuration is shown below:
    /// \code
    /// SerialPort port;
    /// port.setPortName("/dev/ttyS0"); // a serial port in a unix box
    /// port.open();
    /// port.setBaudRate(B9600);
    /// port.setDataFormat(D8N1);
    /// \endcode
    /// until they are complete
    void open();

    void close();
    bool isOpen();
    unsigned int read(std::vector<unsigned char>& buffer);
    unsigned int availableToRead();
    unsigned int write(const std::vector<unsigned char>& buffer);
    IPort::Status waitForRead(int timeoutMs);
    IPort::Status waitForWrite(int timeoutMs);
    void flushRx();
    void flushTx();

private:
    class SerialPortP* _pImpl; //!< platform specific private implementation

}; // SerialPort


} // Grape

#endif // GRAPEIO_SERIALPORT_H
