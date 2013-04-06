//==============================================================================
// Project  : Grape
// Module   : IO Test
// File     : SerialPort_Test.cpp
// Brief    : Serial port test class
//==============================================================================

#include "io/SerialPort.h"
#include <iostream>

int SerialPort_Test()
{
    Grape::SerialPort port;

    port.setPortName("/dev/ttyUSB0");

    if( !port.open() )
    {
        int code;
        std::cerr << port.getLastError(code) << std::endl;
        return -1;
    }

    if( !port.setBaudRate(Grape::SerialPort::B115200) )
    {
        int code;
        std::cerr << port.getLastError(code) << std::endl;
        return -1;
    }

    if( !port.setDataFormat(Grape::SerialPort::D8N1) )
    {
        int code;
        std::cerr << port.getLastError(code) << std::endl;
        return -1;
    }

    std::vector<char> cmd1, cmd2, cmd3, reply;

    // read encoders: 0x55,0xaa,0x10,0x00,0x12,0x21
    cmd1.push_back(0x55);
    cmd1.push_back(0xAA);
    cmd1.push_back(0x10);
    cmd1.push_back(0x00);
    cmd1.push_back(0x12);
    cmd1.push_back(0x21);

    // set to direct power: 0x55,0xAA,0x10,0x01,0x10,0x11,0x31
    cmd2.push_back(0x55);
    cmd2.push_back(0xAA);
    cmd2.push_back(0x10);
    cmd2.push_back(0x01);
    cmd2.push_back(0x10);
    cmd2.push_back(0x11);
    cmd2.push_back(0x31);

    // *dangerous* set full forward power: 0x55,0xaa,0x10,0x02,0x11,0xc8,0xc8,0xb2
    cmd3.push_back(0x55);
    cmd3.push_back(0xAA);
    cmd3.push_back(0x10);
    cmd3.push_back(0x02);
    cmd3.push_back(0x11);
    cmd3.push_back(0xC8);
    cmd3.push_back(0xC8);
    cmd3.push_back(0xB2);

    std::cout << "set direct power..";
    if( port.write(cmd2) )
    {
        std::cerr << "done" << std::endl;
    }
    else
    {
        std::cerr << "failed" << std::endl;
        return -1;
    }

    bool doLoop = true;
    do
    {
        /*
         * Warning: cmd3 causes robot to move
        std::cout << "motors full forward..";
        if( port.write(cmd3) )
        {
            std::cerr << "done" << std::endl;
        }
        else
        {
            std::cerr << "failed" << std::endl;
            return -1;
        }*/

        std::cout << "get encoders..";
        if( port.write(cmd1) )
        {
            std::cerr << "done" << std::endl;
        }
        else
        {
            std::cerr << "failed" << std::endl;
            return -1;
        }

        std::cout << "waiting.." << std::endl;

        reply.clear();
        if(port.waitForRead(5000))
        {
            int nLastRead = 0;
            std::ostringstream text;
            do
            {
                usleep(1000);
                nLastRead = port.read(reply);

                for(int i = 0; i < nLastRead; ++i)
                {
                    text << (std::hex) << (unsigned int)(reply[i]&0xFF) << " ";
                }

            } while ( nLastRead != 0 );

            std::cout << text.str() << std::endl;
        }
        else
        {
            int code = 0;
            std::cout << port.getLastError(code) << std::endl;
            if( code == 0 )
            {
                std::cout << "timeout" << std::endl;
            }
        }

    }while( doLoop );

    return 0;
}
