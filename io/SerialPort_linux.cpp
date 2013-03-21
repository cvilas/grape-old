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
    SerialPortP() : _portFd(-1), _portName("/dev/ttyS0") {}
    ~SerialPortP() {}
public:
    int _portFd;
    std::string _portName;

}; // SerialPortP

// linux baud constants from termios.h
const int SerialPortP::baud[SerialPort::BAUD_MAX] = {B4800, B9600, B19200, B38400, B57600, B115200, B230400};

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
        _errorCode = -1;
        _errorString << "[SerialPort::setBaudRate]: Invalid baud setting" << std::endl;
        return false;
    }

    struct termios tops;
    if( tcgetattr (_pImpl->_portFd, &tops) < 0 )        // get the current attributes
    {
        _errorCode = -1;
        _errorString << "[SerialPort::setBaudRate]: " << strerror(errno) << std::endl;
        return false;
    }

    cfsetispeed (&tops, SerialPortP::baud[baud]); // set input baud rate
    cfsetospeed (&tops, SerialPortP::baud[baud]); // set output baud rate
    tops.c_cflag |= (CLOCAL | CREAD); // enable receiver and set local mode

    if( tcsetattr (_pImpl->_portFd, TCSANOW, &tops) < 0 )       // set new attributes
    {
        _errorCode = -1;
        _errorString << "[SerialPort::setBaudRate]: " << strerror(errno) << std::endl;
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
    _pImpl->_portFd = ::open( _pImpl->_portName.c_str(), O_RDWR | O_NOCTTY /*| O_NDELAY*/);
        // O_NDELAY: don't care what state the DCD line is in

    if( !isOpen() )
    {
        _errorCode = -1;
        _errorString << "[SerialPort::open]: Unable to open " << _pImpl->_portName << std::endl;
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
        _errorCode = errno;
        _errorString << "[SerialPort::read]: " << strerror(errno) << std::endl;
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
        _errorCode = errno;
        _errorString << "[SerialPort::write]: " << strerror(errno) << std::endl;
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
        _errorCode = errno;
        _errorString << "[SerialPort::waitForRead]: " << strerror(errno) << std::endl;
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

/*
//------------------------------------------------------------------------------
bool SerialPort::open(const std::string &port, unsigned int baud)
//------------------------------------------------------------------------------
{
    // get current port settings
    struct termios tops;
    if( tcgetattr (portFd_, &tops) < 0 )        // get the current attributes
    {
        close();
        std::cerr << "[SerialPort::open] Unable to read attributes for " << port << std::endl;
        return false;
    }

    // set parameters for the serial port
    // 230400 baud, 8 data bits (CS8), 1 stop bit(~CSTOPB),
    // no parity(~PARENB), enable receiver (CREAD) and no flow control (~(IXON|IXOFF))

    cfmakeraw(&tops);                                       // set up for binary IO
    // above should be equivalent to:
    //tops.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
    //        | INLCR | IGNCR | ICRNL | IXON);
    //tops.c_oflag &= ~OPOST;
    //tops.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    //tops.c_cflag &= ~(CSIZE | PARENB);
    //tops.c_cflag |= CS8;

    cfsetispeed (&tops, baud);                             // set input baud rate
    cfsetospeed (&tops, baud);                             // set output baud rate

    // debug - minimal
    tops.c_lflag &= ~ECHO; // turn off echo
    tops.c_iflag |= IGNCR; // ignore any CRs

    tops.c_cflag |= (CS8 | CLOCAL | CREAD);                 // 8 data bits, local connection, enable receive

    tops.c_cflag &= ~(IXON|IXOFF);                          // disable flow control
    tops.c_cflag &= ~(CSTOPB);                              // 1 stop bit
    tops.c_cflag &= ~(PARENB);                              // no parity
    tops.c_cc[VMIN] = 0;                                    // timeout (below) to receive the first character
    tops.c_cc[VTIME] = (1 + RECV_TIMEOUT_SEC * 10);      // timeout, in tenth of seconds (100 ms)

    if( tcsetattr (portFd_, TCSANOW, &tops) < 0 )       // set new attributes
    {
        close();
        std::cerr << "[SerialPort::open] Unable to set attributes for " << port << std::endl;
        return false;
    }
    if( tcflush(portFd_, TCIOFLUSH) < 0 )
    {
        close();
        std::cerr << "[SerialPort::open] Unable to flush " << port << std::endl;
        return false;
    }

    //memset(&tops, 0, sizeof(tops));
    //tcgetattr(portFd_, &tops);
    //speed_t baud = cfgetospeed( &tops);

    return true;
}

*/

} // Grape
