//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SerialPort_linux.cpp
// Brief    : Serial port class
//==============================================================================

#include "SerialPort.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <sstream>

#ifdef __ANDROID__
#   define FNDELAY  O_NDELAY
#endif

namespace Grape
{

//==============================================================================
/// \class SerialPortP
/// \brief Linux specific private implementation
//==============================================================================
class SerialPortP
{
public:
    static const int INVALID_PORT_HANDLE = -1;
    static const int baud[SerialPort::BAUD_MAX]; // linux baud rate constants
public:
    SerialPortP() : _portFd(-1), _portName("") {}
    ~SerialPortP() throw() {}
    bool getAttributes(struct termios&);
    bool setAttributes(struct termios&);
public:
    int _portFd;
    std::string _portName;

}; // SerialPortP

// linux baud constants from termios.h
const int SerialPortP::baud[SerialPort::BAUD_MAX] = {B4800, B9600, B19200, B38400, B57600, B115200, B230400};

//==============================================================================
bool SerialPortP::getAttributes(struct termios& tops)
//==============================================================================
{
    return (tcgetattr (_portFd, &tops) == 0 );
}

//------------------------------------------------------------------------------
bool SerialPortP::setAttributes(struct termios& tops)
//------------------------------------------------------------------------------
{
    return ( tcsetattr (_portFd, TCSANOW, &tops) == 0 );
}

//==============================================================================
SerialPort::SerialPort()
//==============================================================================
    : _pImpl(new SerialPortP)
{
}

//------------------------------------------------------------------------------
SerialPort::~SerialPort() throw()
//------------------------------------------------------------------------------
{
    close();
    delete _pImpl;
}

//------------------------------------------------------------------------------
void SerialPort::setPortName(const std::string& port)
//------------------------------------------------------------------------------
{
    if( isOpen() )
    {
        throw SerialPortException(-1, "[SerialPort::setPortName] Port is already open");
    }
    _pImpl->_portName = port;
}

//------------------------------------------------------------------------------
std::string SerialPort::getPortName() const
//------------------------------------------------------------------------------
{
    return _pImpl->_portName;
}

//------------------------------------------------------------------------------
void SerialPort::setBaudRate(BaudRate baud)
//------------------------------------------------------------------------------
{
    if( baud == SerialPort::BAUD_MAX )
    {
        throw InvalidBaudException(-1, "[SerialPort::setBaudRate]: Invalid baud setting");
    }

    struct termios tops;
    if( !_pImpl->getAttributes(tops) )
    {
        throw SerialPortException(-1, "[SerialPort::setBaudRate(getAttributes)] failed");
    }

    cfsetispeed (&tops, SerialPortP::baud[baud]); // set input baud rate
    cfsetospeed (&tops, SerialPortP::baud[baud]); // set output baud rate

    if( !_pImpl->setAttributes(tops) )
    {
        throw SerialPortException(-1, "[SerialPort::setBaudRate(setAttributes)] failed");
    }
}

//------------------------------------------------------------------------------
void SerialPort::setDataFormat(DataFormat fmt)
//------------------------------------------------------------------------------
{
    struct termios tops;
    if( !_pImpl->getAttributes(tops) )
    {
        throw SerialPortException(-1, "[SerialPort::setDataFormat(getAttributes)] failed");
    }

    switch(fmt)
    {
    case D8N1:
        tops.c_cflag &= ~PARENB;
        tops.c_cflag &= ~CSTOPB;
        tops.c_cflag &= ~CSIZE;
        tops.c_cflag |= CS8;
        tops.c_iflag &= ~(INPCK | ISTRIP);
        break;
    case D7E1:
        tops.c_cflag |= PARENB;
        tops.c_cflag &= ~PARODD;
        tops.c_cflag &= ~CSTOPB;
        tops.c_cflag &= ~CSIZE;
        tops.c_cflag |= CS7;
        tops.c_iflag |= (INPCK | ISTRIP);
        break;
    case D7O1:
        tops.c_cflag |= PARENB;
        tops.c_cflag |= PARODD;
        tops.c_cflag &= ~CSTOPB;
        tops.c_cflag &= ~CSIZE;
        tops.c_cflag |= CS7;
        tops.c_iflag |= (INPCK | ISTRIP);
        break;
    default:
        throw InvalidSerialDataFormatException(-1, "[SerialPort::setDataFormat]: Unsupported data format");
    };

    if( !_pImpl->setAttributes(tops) )
    {
        throw SerialPortException(-1, "[SerialPort::setDataFormat(setAttributes)] failed");
    }
}

//------------------------------------------------------------------------------
void SerialPort::open()
//------------------------------------------------------------------------------
{
    close();

    // try to open port
    _pImpl->_portFd = ::open( _pImpl->_portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    // O_NDELAY: don't care what state the DCD signal line is in

    if( !isOpen() )
    {
        std::ostringstream str;
        str << "[SerialPort::open]: Unable to open " << _pImpl->_portName;
        throw IoOpenException(-1, str.str());
    }

    // basic configuration
    // - enable non-blocking read
    // - enable raw data input mode (as opposed to canonical)
    // - enable receiver, local mode
    // - no flow control

    struct termios tops;
    if( !_pImpl->getAttributes(tops) )
    {
        throw SerialPortException(-1, "[SerialPort::Open(getAttributes)] failed");
    }

    fcntl(_pImpl->_portFd, F_SETFL, FNDELAY); // enable non-blocking read

    // timeouts (VTIME) are ignored because we set the NDELAY option above
    tops.c_cc[VMIN] = 0;    // minimum number of characters to read
    tops.c_cc[VTIME] = 0;   // timeout, in tenth of seconds, waiting for first character

    cfmakeraw(&tops); // raw mode

    // the above sets the following terminal attributes
    //termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
    //                | INLCR | IGNCR | ICRNL | IXON);
    //termios_p->c_oflag &= ~OPOST;
    //termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    //termios_p->c_cflag &= ~(CSIZE | PARENB);
    //termios_p->c_cflag |= CS8;

    tops.c_cflag |= (CLOCAL | CREAD); // enable receiver and set local mode
    tops.c_cflag &= ~CRTSCTS; // no hardware flow control
    tops.c_cflag &= ~(IXON | IXOFF | IXANY); // no software flow control

    // apply..
    if( !_pImpl->setAttributes(tops) )
    {
        throw SerialPortException(-1, "[SerialPort::Open(setAttributes)] failed");
    }

    flushRx();
    flushTx();
}

//------------------------------------------------------------------------------
void SerialPort::close() throw()
//------------------------------------------------------------------------------
{
    if( isOpen() )
    {
        ::close( _pImpl->_portFd );
        _pImpl->_portFd = SerialPortP::INVALID_PORT_HANDLE;
    }
}

//------------------------------------------------------------------------------
bool SerialPort::isOpen()
//------------------------------------------------------------------------------
{
    return (_pImpl->_portFd != SerialPortP::INVALID_PORT_HANDLE);
}



//------------------------------------------------------------------------------
unsigned int SerialPort::read(std::vector<unsigned char>& buffer)
//------------------------------------------------------------------------------
{
    unsigned int bytes = availableToRead();

    // ensure output buffer is long enough
    if( bytes > buffer.size() )
    {
        buffer.resize(bytes);
    }

    // read all
    ssize_t bytesRead = ::read(_pImpl->_portFd, &buffer[0], bytes);
    if( bytesRead < 0)
    {
        std::ostringstream str;
        str << "[SerialPort::read]: " << strerror(errno);
        throw IoReadException(errno, str.str());
    }

    return bytesRead;
}

//------------------------------------------------------------------------------
unsigned int SerialPort::availableToRead()
//------------------------------------------------------------------------------
{
    unsigned int bytes = 0;

    if( ioctl(_pImpl->_portFd, FIONREAD, &bytes) < 0 )
    {
        std::ostringstream str;
        str << "[SerialPort::availableToRead]: " << strerror(errno);
        throw SerialPortException(errno, str.str());
    }

    return bytes;
}

//------------------------------------------------------------------------------
unsigned int SerialPort::write(const std::vector<unsigned char>& buffer)
//------------------------------------------------------------------------------
{
    int bytes = ::write(_pImpl->_portFd, &buffer[0], buffer.size());

    if( bytes < 0 )
    {
        std::ostringstream str;
        str << "[SerialPort::write]: " << strerror(errno);
        throw IoWriteException(errno, str.str());
    }

    return bytes;
}

//------------------------------------------------------------------------------
IPort::Status SerialPort::waitForRead(int timeoutMs)
//------------------------------------------------------------------------------
{
    if( !isOpen() )
    {
        throw SerialPortException(-1, "[SerialPort::waitForRead]: Port not open");
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(_pImpl->_portFd, &fds);

    // indefinite wait
    int ret = 0;
    if( timeoutMs < 0 )
    {
        ret = select(_pImpl->_portFd+1, &fds, NULL, NULL, NULL);
    }

    // wait with timeout
    else
    {
        long long int secs = timeoutMs/1000;
        long long int usecs = (timeoutMs * 1000) - (secs * 1000000);
        struct timeval timeout = {secs, usecs};
        ret = select(_pImpl->_portFd+1, &fds, NULL, NULL, &timeout);
    }

    IPort::Status st = IPort::PORT_ERROR;
    // ret == 0: timeout, ret == 1: ready, ret == -1: error
    if (ret > 0)
    {
        st = IPort::PORT_OK;
    }
    else if (ret == 0)
    {
        st = IPort::PORT_TIMEOUT;
    }
    else
    {
        throw IoEventHandlingException(errno, "[SerialPort::waitForRead(select)] failed");
    }

    return st;
}

//------------------------------------------------------------------------------
IPort::Status SerialPort::waitForWrite(int timeoutMs)
//------------------------------------------------------------------------------
{
    if( !isOpen() )
    {
        throw SerialPortException(-1, "[SerialPort::waitForWrite]: Port not open");

    }

    /// \todo: check bytes transmitted

    return IPort::PORT_OK;
}

//------------------------------------------------------------------------------
void SerialPort::flushRx()
//------------------------------------------------------------------------------
{
    tcflush(_pImpl->_portFd, TCIFLUSH);
}

//------------------------------------------------------------------------------
void SerialPort::flushTx()
//------------------------------------------------------------------------------
{
    tcflush(_pImpl->_portFd, TCOFLUSH);
}

} // Grape
