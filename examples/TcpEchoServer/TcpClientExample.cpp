//==============================================================================
// File     :   TcpClientExample.cpp
// Brief    :   Example TCP client program
//==============================================================================

#include "io/TcpSocket.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>

//==============================================================================
std::vector<unsigned char> string2vector(const std::string& str)
//==============================================================================
{
    std::vector<unsigned char> out;
    out.assign(str.begin(), str.end());
    return out;
}

//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{
    if( argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <remoteIp> <port>" << std::endl;
        return -1;
    }

    try
    {
        Grape::TcpSocket client;
        std::string remoteIp(argv[1]);
        int port = atoi(argv[2]);

        // connect to server
        if( !client.connect(remoteIp, port) )
        {
            std::cerr << "Unable to connect to " << remoteIp << ":" << port << std::endl;
            return -1;
        }

        std::cout << "Type something and press enter. Server will echo back" << std::endl;
        while(1)
        {
            std::string is;
            std::getline(std::cin, is);

            if( is.length() )
            {
                // write data to server
                unsigned int bytesWritten = client.write(string2vector(is));

                // wait for server to reply
                std::vector<unsigned char> buffer;
                Grape::IDataPort::Status st = client.waitForRead(1000);
                if( st != Grape::IDataPort::PORT_OK )
                {
                    std::cout << "Error or timeout waiting for reply" << std::endl;
                    break;
                }

                // read the reply
                unsigned int bytesReceived = client.readAll(buffer);

                // server should echo everything back
                if( bytesWritten != bytesReceived )
                {
                    std::cout << "Number of bytes sent and received are not the same!" << std::endl;
                }

                // print reply
                for(unsigned int i = 0; i < bytesReceived; ++i)
                {
                    std::cout << buffer[i];
                }
                std::cout << std::endl;

            } // valid input
        } // while
    }

    catch(Grape::Exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
