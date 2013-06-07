//==============================================================================
// Project  : Grape
// Module   : IO
// File     : TcpSocket.h
// Brief    : TCP socket class
//==============================================================================

#ifndef GRAPE_TCPSOCKET_H
#define GRAPE_TCPSOCKET_H

#ifdef _MSC_VER // using microsoft visual studio
#include <Winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
typedef u_int in_addr_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
typedef unsigned int SOCKET;
#endif

#include "SocketException.h"

namespace Grape
{

class GRAPEIO_DLL_API TcpSocket
{
public:

    /// The default constructor. Initialises socket.
    /// \throw WsaInitException, SocketException
    TcpSocket();

    /// close socket and clean up.
    virtual ~TcpSocket();

    /// close socket
    void close();

    // establish connection with a remote peer
    bool connect(struct sockaddr_in &peer);

    /// bind to a port
    /// \throw SocketException
    void bind(int port);

    /// return socket descriptor
    SOCKET getSocketFd() const { return _sockFd; }

    /// Reset socket descriptor. Note that any previously
    /// set socket properties are lost
    /// (recieve timeout, no delay option, etc)
    void setSocketFd(SOCKET s) { close(); _sockFd = s; }

    /// Resize the TCP send and receive buffer spaces allocated by the OS. Set this
    /// to the bandwidth-delay product for best performance.
    /// About bdp: Bandwidth-delay product in kilo bytes so that socket
    ///            buffers of optimal sizes can be created.
    ///            eg: For a round-trip time (delay between sending a packet
    ///            and receiving acknowledgement) of 50ms, and link bandwidth
    ///            of 100 Mbits per sec, bdp = 100e6 * 50e-3 / 8 = 625 kilo bytes.
    ///            Use 'ping' for approx. measure of round trip time. Specify 0
    ///            for system default.
    /// \throw SocketException
    void setBufSize(unsigned int sz);

    /// set a timeout period on the socket when waiting for messages.
    /// Set ms = 0 to never time out
    /// \throw SocketException
    void setRecvTimeout(unsigned long int ms);

    /// Enable TCP_NODELAY socket option (disable Nagle algorithm)
    /// \throw SocketException
    void setNoDelay(bool option);

    /// Allow opening a port already in use
    /// \throw SocketException
    void allowPortReuse(bool yes);

    /// Send message
    /// \param destAddr   Destination address information
    /// \param outMsgBuf  Pointer to buffer containing your message to server.
    /// \param outMsgLen  The length of your message above.
    /// \throw SocketException
    /// \return number of bytes sent
    unsigned int send(const char *outMsgBuf, unsigned int outMsgLen);

    /// block to receive message or timeout
    /// \param inMsgBuf Buffer to receive message into
    /// \param inBufLen The size (bytes) of the above buffer.
    /// \throw SocketException
    /// \return number of bytes received
    unsigned int receive(char *inMsgBuf, unsigned int inBufLen);

protected:
    SOCKET _sockFd;

}; // TcpSocket

} // Grape

#endif // GRAPE_TCPSOCKET_H
