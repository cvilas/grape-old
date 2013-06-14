//==============================================================================
// File     :   SerialPortMonitor.cpp
// Brief    :   Example program for SerialPort class
//==============================================================================

#include "io/SerialPort.h"
#include "utils/consoleio.h"

#include <iostream>
#include <iomanip>
#include <stdio.h>

//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{
    if( argc != 2 )
    {
        std::cerr << "Usage: " << argv[0] << " <serial port name>" << std::endl;
        return -1;
    }
    try
    {
        Grape::SerialPort port;
        port.setPortName(argv[1]);
        port.open();
        port.setBaudRate(Grape::SerialPort::B9600);
        port.setDataFormat(Grape::SerialPort::D8N1);

        while( !Grape::kbhit() )
        {
            std::vector<unsigned char> buffer;
            if( port.availableToRead() > 0 )
            {
                int nBytes = port.readAll(buffer);
                for(int i = 0; i < nBytes; ++i)
                {
                    std::cout << buffer[i];
                }
            } // bytes available to read
            else
            {
                Grape::milliSleep(500);
            }
        } // until key hit
    } // try
    catch(Grape::Exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return -1;
    }

    return 0;
}

