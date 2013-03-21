//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SerialPort_windows.cpp
// Brief    : Serial port class
//==============================================================================

#include "SerialPort.h"
#include <iostream>
#ifdef _MSC_VER
    #include <Windows.h>
    #define INVALID_PORT_HANDLE INVALID_HANDLE_VALUE
#else
    #include <termios.h>
    #define HANDLE int
    #define INVALID_PORT_HANDLE -1
#endif

//==============================================================================
CSerialPort::CSerialPort()
//==============================================================================
    : portFd_(INVALID_PORT_HANDLE)
{
}

//------------------------------------------------------------------------------
CSerialPort::~CSerialPort()
//------------------------------------------------------------------------------
{
    close();
}

//------------------------------------------------------------------------------
bool CSerialPort::open(const std::string &port, unsigned int baud)
//------------------------------------------------------------------------------
{
    close();

    // try opening the serial connection
    portFd_ = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE,
                         0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if( portFd_ == INVALID_PORT_HANDLE)
    {
        std::cerr << "[SerialPort::open] Unable to open " << port << std::endl;
        return false;
    }

    // set transmit params
    DCB params = {0};
    params.BaudRate = baud;
    params.ByteSize = 8;
    params.StopBits=ONESTOPBIT;
    params.Parity=NOPARITY;

    if( !SetCommState(portFd_, &params) )
    {
        close();
        std::cerr << "[SerialPort::open] SetCommState failed on " << port << std::endl;
        return false;
    }

    // set time out
    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout = 1;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.ReadTotalTimeoutConstant = RECV_TIMEOUT_SEC * 1000; // 20 ms wait
    SetCommTimeouts(portFd_, &timeouts);

    // what did we get?
    GetCommState(portFd_, &params);

    return true;
}

//------------------------------------------------------------------------------
void CSerialPort::close()
//------------------------------------------------------------------------------
{
    if( portFd_ != INVALID_PORT_HANDLE)
    {
        CloseHandle(portFd_);
        portFd_ = INVALID_PORT_HANDLE;
    }
}

//------------------------------------------------------------------------------
bool CSerialPort::read(std::string &st)
//------------------------------------------------------------------------------
{
    st.clear();
    char ch = '\0';

    // read the packet
    /// \todo better way than reading 1 byte at a time?
    do
    {
        DWORD bytesRead = 0;
        if( !ReadFile(portFd_, &ch, 1, &bytesRead, NULL) )
        {
            std::cerr << "[SerialPort::read] Read error " << GetLastError() << std::endl << std::flush;
            return false;
        }

        if (bytesRead < 1 )
        {
            return false;
        }
        st.push_back(ch);
    }while( ch != '\n' );

#ifdef _DEBUG
    std::cout << "[SerialPort::read]: " << st << std::endl << std::flush;
#endif
    return true;
}

//------------------------------------------------------------------------------
bool CSerialPort::write(const std::string &st)
//------------------------------------------------------------------------------
{
    // not required yet

    return false;
}
