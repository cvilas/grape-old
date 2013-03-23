//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SerialPort_linux.cpp
// Brief    : Serial port class
//==============================================================================

#include "SerialPort.h"
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>

#ifndef FNDELAY
#   define FNDELAY  O_NDELAY // required for android
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
    ~SerialPortP() {}
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
SerialPort::~SerialPort()
//------------------------------------------------------------------------------
{
    close();
    delete _pImpl;
}

//------------------------------------------------------------------------------
bool SerialPort::setPortName(const std::string& port)
//------------------------------------------------------------------------------
{
    if( isOpen() )
    {
        return false;
    }
    _pImpl->_portName = port;
    return true;
}

//------------------------------------------------------------------------------
bool SerialPort::setBaudRate(BaudRate baud)
//------------------------------------------------------------------------------
{
    if( baud == SerialPort::BAUD_MAX )
    {
        setError(-1) << "[SerialPort::setBaudRate]: Invalid baud setting" << std::endl;
        return false;
    }

    struct termios tops;
    if( !_pImpl->getAttributes(tops) )
    {
        setError(-1) << "[SerialPort::setBaudRate(getAttributes)]: " << strerror(errno) << std::endl;
        return false;
    }

    cfsetispeed (&tops, SerialPortP::baud[baud]); // set input baud rate
    cfsetospeed (&tops, SerialPortP::baud[baud]); // set output baud rate

    if( !_pImpl->setAttributes(tops) )
    {
        setError(-1) << "[SerialPort::setBaudRate(setAttributes)]: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
bool SerialPort::setDataFormat(DataFormat fmt)
//------------------------------------------------------------------------------
{
    struct termios tops;
    if( !_pImpl->getAttributes(tops) )
    {
        setError(-1) << "[SerialPort::setDataFormat(getAttributes)]: " << strerror(errno) << std::endl;
        return false;
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
        setError(-1) << "[SerialPort::setDataFormat]: Unsupported data format" << std::endl;
        return false;
    };

    if( !_pImpl->setAttributes(tops) )
    {
        setError(-1) << "[SerialPort::setDataFormat(setAttributes)]: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
bool SerialPort::enableHardwareFlowControl(bool enable)
//------------------------------------------------------------------------------
{
    struct termios tops;
    if( !_pImpl->getAttributes(tops) )
    {
        setError(-1) << "[SerialPort::setDataFormat(getAttributes)]: " << strerror(errno) << std::endl;
        return false;
    }

    (enable) ? (tops.c_cflag |= CRTSCTS) : (tops.c_cflag &= ~CRTSCTS);

    if( !_pImpl->setAttributes(tops) )
    {
        setError(-1) << "[SerialPort::setDataFormat(setAttributes)]: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
bool SerialPort::enableSoftwareFlowControl(bool enable, char xon, char xoff)
//------------------------------------------------------------------------------
{
    struct termios tops;
    if( !_pImpl->getAttributes(tops) )
    {
        setError(-1) << "[SerialPort::setDataFormat(getAttributes)]: " << strerror(errno) << std::endl;
        return false;
    }

    if( enable )
    {
        tops.c_cflag |= (IXON | IXOFF | IXANY);
        tops.c_cc[VSTART] = xon;
        tops.c_cc[VSTOP] = xoff;
    }
    else
    {
        tops.c_cflag &= ~(IXON | IXOFF | IXANY);
    }

    if( !_pImpl->setAttributes(tops) )
    {
        setError(-1) << "[SerialPort::setDataFormat(setAttributes)]: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
bool SerialPort::open()
//------------------------------------------------------------------------------
{
    close();

    // try to open port
    _pImpl->_portFd = ::open( _pImpl->_portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    // O_NDELAY: don't care what state the DCD signal line is in

    if( !isOpen() )
    {
        setError(-1) << "[SerialPort::open]: Unable to open " << _pImpl->_portName << std::endl;
        return false;
    }

    // basic configuration
    // - enable non-blocking read
    // - enable raw data input mode (as opposed to canonical)
    // - enable receiver, local mode

    struct termios tops;
    if( !_pImpl->getAttributes(tops) )
    {
        setError(-1) << "[SerialPort::open(getAttributes)]: " << strerror(errno) << std::endl;
        return false;
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
    tops.c_iflag |= IGNCR; // ignore any CRs

    // apply..
    if( !_pImpl->setAttributes(tops) )
    {
        setError(-1) << "[SerialPort::setDataFormat(setAttributes)]: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
void SerialPort::close()
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
int SerialPort::read(std::vector<char>& buffer)
//------------------------------------------------------------------------------
{
    int bytes = 0;

    // find how many bytes are available to read
    if( ioctl(_pImpl->_portFd, FIONREAD, &bytes) < 0 )
    {
        setError(errno) << "[SerialPort::read]: " << strerror(errno) << std::endl;
        return -1;
    }

    // ensure output buffer is long enough
    if( bytes > buffer.size() )
    {
        buffer.resize(bytes);
    }

    // read all
    bytes = ::read(_pImpl->_portFd, &buffer[0], bytes);

    return bytes;
}

//------------------------------------------------------------------------------
int SerialPort::write(const std::vector<char>& buffer)
//------------------------------------------------------------------------------
{
    int bytes = ::write(_pImpl->_portFd, &buffer[0], buffer.size());

    if( bytes < 0 )
    {
        setError(errno) << "[SerialPort::write]: " << strerror(errno) << std::endl;
    }

    return bytes;
}

//------------------------------------------------------------------------------
bool SerialPort::waitForRead(int timeoutMs)
//------------------------------------------------------------------------------
{
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

    // ret == 0: timeout, ret == 1: ready, ret == -1: error
    if( ret < 0)
    {
        setError(errno) << "[SerialPort::waitForRead]: " << strerror(errno) << std::endl;
    }

    return (ret > 0);
}

//------------------------------------------------------------------------------
bool SerialPort::waitForWrite(int timeoutMs)
//------------------------------------------------------------------------------
{
    /// \todo: check if we have flow control enabled. if so, wait until remote
    ///        is ready to receive. if no flow control, just return true. We
    ///        can't really do anything meaningful.

    return true;
}

} // Grape
