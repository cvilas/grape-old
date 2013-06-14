//==============================================================================
// File     :   TcpEchoServer.cpp
// Brief    :   Example TCP server
//==============================================================================

#include <iostream>
#include <io/TcpSocket.h>
#include <QThread>

//==============================================================================
/// \brief services a socket connection in a separate thread
class ClientServiceThread : public QThread
//==============================================================================
{

public:

    ClientServiceThread(Grape::TcpSocket* socket) : QThread(), _pSocket(socket) {}

    ~ClientServiceThread() { delete _pSocket; }

    void print(std::vector<unsigned char> &b, bool inHex)
    {
        if( b.size() )
        {
            std::vector<unsigned char>::const_iterator it = b.begin();
            while(it != b.end() )
            {
                if(inHex)
                    std::cout << std::hex << (int)*it;
                else
                    std::cout << *it;
                ++it;
            }
            std::cout << std::endl;
        }

    }

    // the thread function
    void run()
    {
        std::cout << "Connected to client " << _pSocket->getPeerName() << std::endl;

        try
        {
            std::vector<unsigned char> buffer;

            do
            {
                // wait until there is something to read
                Grape::IDataPort::Status st = _pSocket->waitForRead(1000);

                // read and echo back
                if( st == Grape::IDataPort::PORT_OK )
                {
                    buffer.clear();

                    _pSocket->readAll(buffer);
                    print(buffer,false);
                    _pSocket->write(buffer);
                }
                //else if( st == Grape::IDataPort::PORT_TIMEOUT )
                //{
                //    std::cout << "timeout" << std::endl;
                //}
                else if( st == Grape::IDataPort::PORT_ERROR )
                {
                    std::cout << "comm error." << std::endl;
                    break;
                }
            }while(1);
        } // try

        catch(Grape::Exception &ex)
        {
            std::cout << ex.what() << std::endl;
        }

        // close and exit
        _pSocket->close();
        std::cout << "Exit client service thread" << std::endl;
    }

private:
    Grape::TcpSocket* _pSocket;
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

    // bind to a port so remote hosts can connect to us
    server.bind(port);

    std::cout << "TcpServer ready on port " << port << std::endl;

    // start listening for connection requests
    server.listen(100);

    while(1)
    {
        try
        {
            // wait for and accept a new connection from remote host
            Grape::TcpSocket* client = server.accept();

            // service the remote host in a separate thread
            ClientServiceThread* serviceThread = new ClientServiceThread(client);
            serviceThread->start();
        }

        catch(Grape::Exception& ex)
        {
            std::cout << ex.what() << std::endl;
            return -1;
        }
    }
    return 0;
}
