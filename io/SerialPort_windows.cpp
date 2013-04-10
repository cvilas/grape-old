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

/// \note For a guide to serial port programming on windows, see
/// http://msdn.microsoft.com/en-us/library/ms810467.aspx

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
    int waitForReadWrite(bool isRead, int timeoutMs);
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

//------------------------------------------------------------------------------
int SerialPortP::waitForReadWrite(bool isRead, int timeoutMs)
//------------------------------------------------------------------------------
{
    OVERLAPPED osReader = {0};
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if( osReader.hEvent == NULL )
    {
        //setError(-1) << "[SerialPort::waitForRead]: CreateEvent failed" << std::endl;
        return -1;
    }

    DWORD ev = (isRead)?(EV_RXCHAR):(EV_TXEMPTY);
    if( 0 == WaitCommEvent(_portFd, &ev, &osReader) )
    {
        CloseHandle(osReader.hEvent);
        //setError(GetLastError()) << "[SerialPort::waitForRead]: WaitCommEvent failed" << std::endl;
        return -1;
    }

    int result = -1;
    DWORD waitResult = WaitForSingleObject(osReader.hEvent, timeoutMs);
    switch(waitResult)
    {
    case WAIT_OBJECT_0:
        DWORD bytes;
        if( 0 == GetOverlappedResult(_portFd, &osReader, &bytes, TRUE) )
        {
            //setError(GetLastError()) << "[SerialPort::waitForRead]: GetOverlappedResult failed" << std::endl;
            result = -1;
        }
        else
        {
            result = 1;
        }
        break;
    case WAIT_TIMEOUT:
        result = 0;
        break;
    default:
        //setError(GetLastError()) << "[SerialPort::waitForRead]: WaitForSingleObject failed" << std::endl;
        result = -1;
        break;
    }

    CloseHandle(osReader.hEvent);

    return result;
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
        setError(GetLastError()) << "[SerialPort::setDataFormat]: getAttributes failed" << std::endl;
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
        setError(GetLastError()) << "[SerialPort::setDataFormat]: setAttributes failed" << std::endl;
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
                                 0, 0,
                                 OPEN_EXISTING,
                                 FILE_FLAG_OVERLAPPED,
                                 0);
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
        setError(GetLastError()) << "[SerialPort::open]: getAttributes failed" << std::endl;
        return false;
    }

    dcb.fBinary = TRUE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fNull = FALSE;
    dcb.fAbortOnError = FALSE;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fErrorChar = FALSE;

    // apply..
    if( !_pImpl->setAttributes(dcb) )
    {
        setError(GetLastError()) << "[SerialPort::open]: setAttributes failed" << std::endl;
        return false;
    }

    // set time out
    /// \todo Are these timeouts correct for overlapped IO operations?
    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;

    if( 0 == SetCommTimeouts(_pImpl->_portFd, &timeouts) )
    {
        setError(GetLastError()) << "[SerialPort::open]: SetCommTimeouts failed" << std::endl;
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
    // how many have we got to read
    int bytesToRead = availableToRead();
    if( bytesToRead < 0 )
    {
        return -1;
    }

    // ensure output buffer is long enough
    if( bytesToRead > buffer.size() )
    {
        buffer.resize(bytesToRead);
    }

    // create event to handle read completion
    OVERLAPPED osReader = {0};
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if( osReader.hEvent == NULL )
    {
        setError(-1) << "[SerialPort::read]: CreateEvent failed" << std::endl;
        return -1;
    }

    // read all
    int bytesRead = 0;
    if( 0 != ReadFile(_pImpl->_portFd, &buffer[0], bytesToRead, (LPDWORD)(&bytesRead), &osReader) )
    {
        // read completed immediately. We are done
        CloseHandle(osReader.hEvent);
        return bytesRead;
    }

    // ReadFile returned false. Could be due to pending IO or an error
    DWORD lastErr = GetLastError();
    if( lastErr != ERROR_IO_PENDING ) // not pending IO.
    {
        CloseHandle(osReader.hEvent);
        setError(lastErr) << "[SerialPort::read]: ReadFile failed" << std::endl;
        return -1;
    }

    // We have a pending IO. Wait for read to complete
    bool success = false;
    DWORD dwRes = WaitForSingleObject(osReader.hEvent, INFINITE);
    switch(dwRes)
    {
    case WAIT_OBJECT_0: // Read Completed
        if( 0 == GetOverlappedResult(_pImpl->_portFd, &osReader, (LPDWORD)(&bytesRead), TRUE) )
        {
            setError(GetLastError()) << "[SerialPort::read]: GetOverlappedResult failed" << std::endl;
        }
        else
        {
            success = true;
        }
        break;
    case WAIT_TIMEOUT:
        /* can't happen, considering we are in INFINITE wait */
        setError(-1) << "[SerialPort::read]: WaitForSingleObject timed out" << std::endl;
        break;
    default:
        setError(-1) << "[SerialPort::read]: WaitForSingleObject failed" << std::endl;
        break;
    }

    CloseHandle(osReader.hEvent);
    return (success)?(bytesRead):(-1);
}

//------------------------------------------------------------------------------
int SerialPort::availableToRead()
//------------------------------------------------------------------------------
{
    COMSTAT stat;
    DWORD errors;
    BOOL ret = ClearCommError(_pImpl->_portFd, &errors, &stat);
    if( ret == 0)
    {
        setError(GetLastError()) << "[SerialPort::availableToRead]: ClearCommError failed" << std::endl;
        return -1;
    }
    return stat.cbInQue;
}

//------------------------------------------------------------------------------
int SerialPort::write(const std::vector<char>& buffer)
//------------------------------------------------------------------------------
{
    // create event to handle write completion
    OVERLAPPED osWriter = {0};
    osWriter.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if( osWriter.hEvent == NULL )
    {
        setError(-1) << "[SerialPort::write]: CreateEvent failed" << std::endl;
        return -1;
    }

    // write all
    int bytesWritten = 0;
    if( 0 != WriteFile(_pImpl->_portFd, &buffer[0], buffer.size(), (LPDWORD)(&bytesWritten), &osWriter) )
    {
        // completed immediately. We are done
        CloseHandle(osWriter.hEvent);
        return bytesWritten;
    }

    // WriteFile returned false. Could be due to pending IO or an error
    DWORD lastErr = GetLastError();
    if( lastErr != ERROR_IO_PENDING ) // not pending IO.
    {
        CloseHandle(osWriter.hEvent);
        setError(lastErr) << "[SerialPort::write]: WriteFile failed" << std::endl;
        return -1;
    }

    // We have a pending IO. Wait for read to complete
    bool success = false;
    DWORD dwRes = WaitForSingleObject(osWriter.hEvent, INFINITE);
    switch(dwRes)
    {
    case WAIT_OBJECT_0: // Read Completed
        if( 0 == GetOverlappedResult(_pImpl->_portFd, &osWriter, (LPDWORD)(&bytesWritten), TRUE) )
        {
            setError(GetLastError()) << "[SerialPort::write]: GetOverlappedResult failed" << std::endl;
        }
        else
        {
            success = true;
        }
        break;
    case WAIT_TIMEOUT:
        /* can't happen, considering we are in INFINITE wait */
        setError(-1) << "[SerialPort::write]: WaitForSingleObject timed out" << std::endl;
        break;
    default:
        setError(-1) << "[SerialPort::write]: WaitForSingleObject failed" << std::endl;
        break;
    }

    CloseHandle(osWriter.hEvent);
    return (success)?(bytesWritten):(-1);
}

//------------------------------------------------------------------------------
int SerialPort::waitForRead(int timeoutMs)
//------------------------------------------------------------------------------
{
    return _pImpl->waitForReadWrite(true, timeoutMs);
}

//------------------------------------------------------------------------------
int SerialPort::waitForWrite(int timeoutMs)
//------------------------------------------------------------------------------
{
    return _pImpl->waitForReadWrite(false, timeoutMs);
}




} // Grape

