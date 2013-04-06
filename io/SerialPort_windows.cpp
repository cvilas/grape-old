//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SerialPort_windows.cpp
// Brief    : Serial port class
//==============================================================================

#include "SerialPort.h"
#include <Windows.h>

namespace Grape
{

//==============================================================================
/// \class SerialPortP
/// \brief Windows specific private implementation
//==============================================================================
class SerialPortP
{
public:
    static const int baud[SerialPort::BAUD_MAX]; // windows baud rate constants
public:
    SerialPortP() : _portFd(INVALID_HANDLE_VALUE), _portName("") {}
    ~SerialPortP() {}
    bool getAttributes(DCB&);
    bool setAttributes(DCB&);
public:
    HANDLE _portFd;
    std::string _portName;

}; // SerialPortP

// windows baud constants from winbase.h
const int SerialPortP::baud[SerialPort::BAUD_MAX] = {CBR_4800, CBR_9600, CBR_19200, CBR_38400, CBR_57600, CBR_115200, 230400};

//==============================================================================
bool SerialPortP::getAttributes(DCB& dcb)
//==============================================================================
{
    return (GetCommState(_portFd, &dcb) != 0);
}

//------------------------------------------------------------------------------
bool SerialPortP::setAttributes(DCB& dcb)
//------------------------------------------------------------------------------
{
    dcb.DCBlength = sizeof(DCB);
    return (SetCommState(_portFd, &dcb) != 0);
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
std::string SerialPort::getPortName() const
//------------------------------------------------------------------------------
{
    return _pImpl->_portName;
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

    DCB dcb = {0};
    if( !_pImpl->getAttributes(dcb) )
    {
        setError(GetLastError()) << "[SerialPort::setBaudRate]: Unable to get port attributes" << std::endl;
        return false;
    }

    dcb.BaudRate = SerialPortP::baud[baud];

    if( !_pImpl->setAttributes(dcb) )
    {
        setError(GetLastError()) << "[SerialPort::setBaudRate: Unabl to set port attributes" << std::endl;
        return false;
    }

    return true;

}

//------------------------------------------------------------------------------
bool SerialPort::setDataFormat(DataFormat fmt)
//------------------------------------------------------------------------------
{
    DCB dcb;
    if( !_pImpl->getAttributes(dcb) )
    {
        setError(GetLastError()) << "[SerialPort::setDataFormat]: Unable to get port attributes" << std::endl;
        return false;
    }

    switch(fmt)
    {
    case D8N1:
        dcb.ByteSize =  8;
        dcb.Parity = NOPARITY;
        dcb.StopBits = ONESTOPBIT;
        dcb.fParity = FALSE;
        dcb.fErrorChar = FALSE;
        break;
    case D7E1:
        dcb.ByteSize =  7;
        dcb.Parity = EVENPARITY;
        dcb.StopBits = ONESTOPBIT;
        dcb.fParity = TRUE;
        dcb.fErrorChar = FALSE;
        break;
    case D7O1:
        dcb.ByteSize =  7;
        dcb.Parity = ODDPARITY;
        dcb.StopBits = ONESTOPBIT;
        dcb.fParity = TRUE;
        dcb.fErrorChar = FALSE;
        break;
    default:
        setError(-1) << "[SerialPort::setDataFormat]: Unsupported data format" << std::endl;
        return false;
    };

    if( !_pImpl->setAttributes(dcb) )
    {
        setError(GetLastError()) << "[SerialPort::setDataFormat]: Unable to set port attributes" << std::endl;
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
bool SerialPort::enableHardwareFlowControl(bool enable)
//------------------------------------------------------------------------------
{
    DCB dcb;
    if( !_pImpl->getAttributes(dcb) )
    {
        setError(GetLastError()) << "[SerialPort::enableHardwareFlowControl]: Unable to set port attributes" << std::endl;
        return false;
    }

    dcb.fOutxCtsFlow = enable;
    dcb.fOutxDsrFlow = enable;

    if( !_pImpl->setAttributes(dcb) )
    {
        setError(GetLastError()) << "[SerialPort::enableHardwareFlowControl]: Unable to set port attributes" << std::endl;
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
bool SerialPort::enableSoftwareFlowControl(bool enable, char xon, char xoff)
//------------------------------------------------------------------------------
{
    DCB dcb;
    if( !_pImpl->getAttributes(dcb) )
    {
        setError(GetLastError()) << "[SerialPort::enableSoftwareFlowControl]: Unable to set port attributes" << std::endl;
        return false;
    }

    if( enable )
    {
        dcb.fOutX = TRUE;
        dcb.fInX = TRUE;
        dcb.XonChar = xon;
        dcb.XoffChar = xoff;
    }
    else
    {
        dcb.fOutX = FALSE;
        dcb.fInX = FALSE;
    }

    if( !_pImpl->setAttributes(dcb) )
    {
        setError(GetLastError()) << "[SerialPort::enableSoftwareFlowControl]: Unable to set port attributes" << std::endl;
        return false;
    }

    return true;

}

//------------------------------------------------------------------------------
bool SerialPort::open()
//------------------------------------------------------------------------------
{
    close();

    // try opening the serial connection
    _pImpl->_portFd = CreateFile(_pImpl->_portName.c_str(),
                                 GENERIC_READ | GENERIC_WRITE,
                                 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if( !isOpen() )
    {
        setError(-1) << "[SerialPort::open]: Unable to open " << _pImpl->_portName << std::endl;
        return false;
    }

    // basic configuration
    // - enable non-blocking read
    // - enable raw data input mode (as opposed to canonical)
    // - enable receiver, local mode
    // - no flow control

    DCB dcb;
    if( !_pImpl->getAttributes(dcb) )
    {
        setError(GetLastError()) << "[SerialPort::open]: Unable to get port attributes" << std::endl;
        return false;
    }

    dcb.fBinary = TRUE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fNull = FALSE;
    dcb.fAbortOnError = FALSE;

    // apply..
    if( !_pImpl->setAttributes(dcb) )
    {
        setError(GetLastError()) << "[SerialPort::open]: Unable to set port attributes" << std::endl;
        return false;
    }

    // comm events
    if( !SetCommMask(_pImpl->_portFd, EV_TXEMPTY|EV_RXCHAR) )
    {
        setError(GetLastError()) << "[SerialPort::open]: Unable to set port event parameters" << std::endl;
        return false;

    }

    // set time out
    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;

    if( !SetCommTimeouts(_pImpl->_portFd, &timeouts) )
    {
        setError(GetLastError()) << "[SerialPort::open]: Unable to set timeout attributes" << std::endl;
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
        CloseHandle(_pImpl->_portFd);
        _pImpl->_portFd = INVALID_HANDLE_VALUE;
    }
}

//------------------------------------------------------------------------------
bool SerialPort::isOpen()
//------------------------------------------------------------------------------
{
    return (_pImpl->_portFd != INVALID_HANDLE_VALUE);
}

//------------------------------------------------------------------------------
int SerialPort::read(std::vector<char>& buffer)
//------------------------------------------------------------------------------
{
    /// \todo
    //if( !ReadFile(portFd_, &ch, 1, &bytesRead, NULL) )
    return -1;
}

//------------------------------------------------------------------------------
int SerialPort::availableToRead()
//------------------------------------------------------------------------------
{
    return -1;
}

//------------------------------------------------------------------------------
int SerialPort::write(const std::vector<char>& buffer)
//------------------------------------------------------------------------------
{
    /// \todo
    return -1;
}

//------------------------------------------------------------------------------
bool SerialPort::waitForRead(int timeoutMs)
//------------------------------------------------------------------------------
{
    /// \todo
    //WaitCommEvent(_pImpl->_portFd, )
    return true;
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

