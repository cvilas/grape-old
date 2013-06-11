//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SerialPort_windows.cpp
// Brief    : Serial port class
//==============================================================================

#include "SerialPort.h"
#include <Windows.h>
#include <sstream>

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
    SerialPortP()
        : _portFd(INVALID_HANDLE_VALUE),
          _portName("")
    {}
    ~SerialPortP() throw() {}
    bool getAttributes(DCB&);
    bool setAttributes(DCB&);
    IPort::Status waitForReadWrite(bool isRead, int timeoutMs);
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
IPort::Status SerialPortP::waitForReadWrite(bool isRead, int timeoutMs)
//------------------------------------------------------------------------------
{
    OVERLAPPED osReader = {0};
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if( osReader.hEvent == NULL )
    {
        throw IoEventHandlingException(-1, "[SerialPort::waitForRead(CreateEvent)] failed");
    }

    DWORD ev = (isRead)?(EV_RXCHAR):(EV_TXEMPTY);
    if( 0 == WaitCommEvent(_portFd, &ev, &osReader) )
    {
        CloseHandle(osReader.hEvent);
        throw IoEventHandlingException(-1, "[SerialPort::waitForRead(WaitCommEvent)] failed");
    }

    IPort::Status result = IPort::PORT_ERROR;
    DWORD waitResult = WaitForSingleObject(osReader.hEvent, timeoutMs);
    switch(waitResult)
    {
    case WAIT_OBJECT_0:
        DWORD bytes;
        if( 0 == GetOverlappedResult(_portFd, &osReader, &bytes, TRUE) )
        {
            throw IoEventHandlingException(-1, "[SerialPort::waitForRead(GetOverlappedResult)] failed");
        }
        else
        {
            result = IPort::PORT_OK;
        }
        break;
    case WAIT_TIMEOUT:
        result = IPort::PORT_TIMEOUT;
        break;
    default:
        throw IoEventHandlingException(-1, "[SerialPort::waitForRead(WaitForSingleObject)] failed");
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

    DCB dcb = {0};
    if( !_pImpl->getAttributes(dcb) )
    {
        throw SerialPortException(GetLastError(), "[SerialPort::setBaudRate(getAttributes)] failed");
    }

    dcb.BaudRate = SerialPortP::baud[baud];

    if( !_pImpl->setAttributes(dcb) )
    {
        throw SerialPortException(GetLastError(), "[SerialPort::setBaudRate(setAttributes)] failed");
    }
}

//------------------------------------------------------------------------------
void SerialPort::setDataFormat(DataFormat fmt)
//------------------------------------------------------------------------------
{
    DCB dcb;
    if( !_pImpl->getAttributes(dcb) )
    {
        throw SerialPortException(GetLastError(), "[SerialPort::setDataFormat(getAttributes)] failed");
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
        throw InvalidSerialDataFormatException(-1, "[SerialPort::setDataFormat]: Unsupported data format");
    };

    if( !_pImpl->setAttributes(dcb) )
    {
        throw SerialPortException(GetLastError(), "[SerialPort::setDataFormat(setAttributes)] failed");
    }
}

//------------------------------------------------------------------------------
void SerialPort::open()
//------------------------------------------------------------------------------
{
    close();

    // try opening the serial connection
    std::wstring portName(_pImpl->_portName.begin(), _pImpl->_portName.end());
    _pImpl->_portFd = CreateFile(portName.c_str(),
                                 GENERIC_READ | GENERIC_WRITE,
                                 0, 0,
                                 OPEN_EXISTING,
                                 FILE_FLAG_OVERLAPPED,
                                 0);
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

    DCB dcb;
    if( !_pImpl->getAttributes(dcb) )
    {
        throw SerialPortException(GetLastError(), "[SerialPort::Open(getAttributes)] failed");
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
        throw SerialPortException(GetLastError(), "[SerialPort::Open(setAttributes)] failed");
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
        throw SerialPortException(GetLastError(), "[SerialPort::Open(SetCommTimeouts)] failed");
    }
}

//------------------------------------------------------------------------------
void SerialPort::close() throw()
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
unsigned int SerialPort::read(std::vector<unsigned char>& buffer)
//------------------------------------------------------------------------------
{
    // how many have we got to read
    unsigned int bytesToRead = availableToRead();
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
        throw IoEventHandlingException(-1, "[SerialPort::read(CreateEvent)] failed");
    }

    // read all
    unsigned int bytesRead = 0;
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
        throw IoReadException(lastErr, "[SerialPort::read(ReadFile)] failed");
    }

    // We have a pending IO. Wait for read to complete
    bool success = false;
    DWORD dwRes = WaitForSingleObject(osReader.hEvent, INFINITE);
    switch(dwRes)
    {
    case WAIT_OBJECT_0: // Read Completed
        if( 0 == GetOverlappedResult(_pImpl->_portFd, &osReader, (LPDWORD)(&bytesRead), TRUE) )
        {
            throw IoEventHandlingException(GetLastError(), "[SerialPort::read(GetOverlappedResult)] failed");
        }
        else
        {
            success = true;
        }
        break;
    case WAIT_TIMEOUT:
        /* can't happen, considering we are in INFINITE wait */
        throw IoEventHandlingException(-1, "[SerialPort::read(WaitForSingleObject)] timed out");
        break;
    default:
        throw IoEventHandlingException(-1, "[SerialPort::read(WaitForSingleObject)] failed");
        break;
    }

    CloseHandle(osReader.hEvent);
    return bytesRead;
}

//------------------------------------------------------------------------------
unsigned int SerialPort::availableToRead()
//------------------------------------------------------------------------------
{
    COMSTAT stat;
    DWORD errors;
    BOOL ret = ClearCommError(_pImpl->_portFd, &errors, &stat);
    if( ret == 0)
    {
        throw SerialPortException(GetLastError(), "[SerialPort::availableToRead(ClearCommError)] failed");
    }
    return stat.cbInQue;
}

//------------------------------------------------------------------------------
unsigned int SerialPort::write(const std::vector<unsigned char>& buffer)
//------------------------------------------------------------------------------
{
    // create event to handle write completion
    OVERLAPPED osWriter = {0};
    osWriter.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if( osWriter.hEvent == NULL )
    {
        throw IoEventHandlingException(-1, "[SerialPort::write(CreateEvent)] failed");
    }

    // write all
    unsigned int bytesWritten = 0;
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
        throw IoWriteException(lastErr, "[SerialPort::write(WriteFile)] failed");
    }

    // We have a pending IO. Wait for read to complete
    bool success = false;
    DWORD dwRes = WaitForSingleObject(osWriter.hEvent, INFINITE);
    switch(dwRes)
    {
    case WAIT_OBJECT_0: // Read Completed
        if( 0 == GetOverlappedResult(_pImpl->_portFd, &osWriter, (LPDWORD)(&bytesWritten), TRUE) )
        {
            throw IoEventHandlingException(GetLastError(), "[SerialPort::write(GetOverlappedResult)] failed");
        }
        else
        {
            success = true;
        }
        break;
    case WAIT_TIMEOUT:
        /* can't happen, considering we are in INFINITE wait */
        throw IoEventHandlingException(-1, "[SerialPort::write(WaitForSingleObject)] timed out");
        break;
    default:
        throw IoEventHandlingException(-1, "[SerialPort::write(WaitForSingleObject)] failed");
        break;
    }

    CloseHandle(osWriter.hEvent);
    return bytesWritten;
}

//------------------------------------------------------------------------------
IPort::Status SerialPort::waitForRead(int timeoutMs)
//------------------------------------------------------------------------------
{
    return _pImpl->waitForReadWrite(true, timeoutMs);
}

//------------------------------------------------------------------------------
IPort::Status SerialPort::waitForWrite(int timeoutMs)
//------------------------------------------------------------------------------
{
    return _pImpl->waitForReadWrite(false, timeoutMs);
}

//------------------------------------------------------------------------------
void SerialPort::flushRx()
//------------------------------------------------------------------------------
{
    PurgeComm(_pImpl->_portFd, PURGE_RXABORT|PURGE_RXCLEAR);
}

//------------------------------------------------------------------------------
void SerialPort::flushTx()
//------------------------------------------------------------------------------
{
    PurgeComm(_pImpl->_portFd, PURGE_TXABORT|PURGE_TXCLEAR);
}


} // Grape

