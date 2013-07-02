//==============================================================================
// File     :   SerialPortMonitor.cpp
// Brief    :   Example program for SerialPort class
//==============================================================================

#include "io/SerialPort.h"
#include "utils/consoleio.h"
#include <iostream>
#include <string>
#include <stdlib.h>

//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{
    if( argc != 3 )
    {
        std::cerr << "Usage: " << argv[0] << " <serial port name> <r=0, w=2, rw=3>" << std::endl;
        return -1;
    }
    try
    {
        Grape::SerialPort port;
        port.setPortName(argv[1]);
        port.open();
        port.setBaudRate(Grape::SerialPort::B9600);
        port.setDataFormat(Grape::SerialPort::D8N1);

        int rwOpt = atoi(argv[2]);

        while( /*!Grape::kbhit()*/1 )
        {
            if( (rwOpt == 2) || (rwOpt == 3) )
            {
                // read input from user
                std::string userin;
                std::getline(std::cin, userin);

                // send to remote device
                std::vector<unsigned char> bb(userin.begin(), userin.end());
                bb.push_back('\n');
                port.write(bb);
            }

            if( (rwOpt == 0) || (rwOpt == 3) )
            {
                // read response from remote device
                if( Grape::IDataPort::PORT_OK == port.waitForRead(1000) )
                {
                    std::vector<unsigned char> buffer;
                    int nBytes = port.readAll(buffer);
                    for(int i = 0; i < nBytes; ++i)
                    {
                        std::cout << (char)buffer[i] << std::flush;
                    }
                } // bytes available to read
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

