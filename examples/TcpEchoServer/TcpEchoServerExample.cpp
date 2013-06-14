//==============================================================================
// File     :   TcpEchoServer.cpp
// Brief    :   Example TCP server
//==============================================================================

#include <iostream>
#include <io/TcpSocket.h>
#include <QThread>

//==============================================================================
class ClientServiceThread : public QThread
//==============================================================================
{
public:
    ClientServiceThread(Grape::TcpSocket& socket) : QThread(), _socket(socket) {}
    void run()
    {
        std::cout << "Connected to client " << std::endl;

        try
        {
            std::vector<unsigned char> buffer;

            do
            {
                Grape::IDataPort::Status st = _socket.waitForRead(10000);
                if( st == Grape::IDataPort::PORT_OK )
                {
                    buffer.clear();
                    _socket.readAll(buffer);
                    std::vector<unsigned char>::const_iterator it = buffer.begin();
                    while(it != buffer.end() )
                    {
                        std::cout << *it;
                        ++it;
                    }
                    _socket.write(buffer);
                }
                else if( st == Grape::IDataPort::PORT_ERROR )
                {
                    std::cout << "Error waiting for data." << std::endl;
                    break;
                }
            }while(1);
        } // try

        catch(Grape::Exception &ex)
        {
            std::cout << ex.what() << std::endl;
        }
        _socket.close();
        std::cout << "Exiting thread" << std::endl;
    }
private:
    Grape::TcpSocket& _socket;
};

//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{
    if( argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return -1;
    }
    int port = atoi(argv[1]);

    Grape::TcpSocket server;
    server.allowPortReuse(true);
    server.bind(port);
    std::cout << "TcpServer ready on port " << port << std::endl;

    server.listen(100);

    while(1)
    {
        try
        {
            Grape::TcpSocket client = server.accept();
            ClientServiceThread serviceThread(client);
            serviceThread.start();
            serviceThread.wait();
            client.close();
        }
        catch(Grape::Exception& ex)
        {
            std::cout << ex.what() << std::endl;
            return -1;
        }
    }
    return 0;
}
