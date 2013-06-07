//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IpSocket.h
// Brief    : Base class for IP socket
//==============================================================================

#ifndef GRAPE_IPSOCKET_H
#define GRAPE_IPSOCKET_H

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

/// \brief Base class for BSD sockets
/// \ingroup io
///
/// See derived classes for specific implementations.
/// \todo
/// - setSocketFd() should check that socket of the correct type is set (TCP, UDP, etc)
class GRAPEIO_DLL_API IpSocket
{
public:

    /// \brief supported socket types
    enum SocketType
    {
        TCP,    //!< TcpSocket
        UDP     //!< UdpSocket
    };

public:

    virtual ~IpSocket() throw(/*nothing*/);

    /// Reset raw socket descriptor. Note that any previously
    /// set socket properties are lost
    /// (recieve timeout, no delay option, etc)
    void setSocketFd(SOCKET s) { close(); _sockFd = s; }

    /// return socket descriptor
    SOCKET getSocketFd() const { return _sockFd; }

    /// establish connection with a remote peer
    virtual bool connect(struct sockaddr_in &peer) = 0;

    /// close socket
    void close();

    /// bind socket to a port
    /// \throw SocketException
    void bind(int port);

    /// Resize the send and receive buffer spaces allocated by the OS. Set this
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

    /// Allow opening a port already in use
    /// \throw SocketException
    void allowPortReuse(bool yes);

    /// Send message to connected remote peer
    /// \param outMsgBuf  Pointer to buffer containing your message to remote host.
    /// \param outMsgLen  The length of your message above.
    /// \throw SocketException
    /// \return number of bytes sent
    /// \see connect
    virtual unsigned int send(const char *outMsgBuf, unsigned int outMsgLen) = 0;

    /// block to receive message from remote peer or timeout
    /// \param inMsgBuf Buffer to receive message into
    /// \param inBufLen The size (bytes) of the above buffer.
    /// \throw SocketException
    /// \return number of bytes received
    virtual unsigned int receive(char *inMsgBuf, unsigned int inBufLen) = 0;

protected:

    /// Base class contructor initialises socket library. Derived classes
    /// initialise socket.
    /// \throw WsaInitException, SocketException
    IpSocket();

    /// Throw a SocketException. Specify location from where it was thrown in
    /// order to help the user
    void throwSocketException(const std::string& location);

protected:
    SOCKET _sockFd;

}; // IpSocket

} // Grape

#endif // GRAPE_IPSOCKET_H
