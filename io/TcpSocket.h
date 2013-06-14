//==============================================================================
// Project  : Grape
// Module   : IO
// File     : TcpSocket.h
// Brief    : TCP socket class
//==============================================================================

#ifndef GRAPE_TCPSOCKET_H
#define GRAPE_TCPSOCKET_H

#include "IpSocket.h"

namespace Grape
{

/// \brief TCP socket
/// \ingroup io
///
/// Note that any method can throw SocketException on error
///
/// Threaded TCP server example:
/// \include TcpEchoServerExample.cpp
///
/// TCP client example:
/// \include TcpClientExample.cpp
class GRAPEIO_DLL_API TcpSocket : public IpSocket
{
public:

    TcpSocket();
    virtual ~TcpSocket() throw(/*nothing*/);

    /// Enable TCP_NODELAY socket option (disable Nagle algorithm)
    /// \throw SocketException
    void setNoDelay(bool option);

    /// Establish connection with remote peer. This is only for connection based
    /// protocols such as TCP
    /// \param remoteIp Remote host IP
    /// \param remotePort Port to connect to on remote host
    /// \return true if connection was established successfully
    bool connect(const std::string& remoteIp,int remotePort);

    /// Place socket in a passive state listening for incoming connections.
    /// Note that this will work only with connection-based protocols such as TCP.
    /// Call bind first.
    /// \param backlog The maximum length of queue of pending connections.
    /// \see bind, accept
    void listen(int backlog);

    /// Use this with connection-based protocols (eg: TCP) to accept an incoming
    /// connection. For this to work, this socket should be bound to a port (see bind())
    /// and listening for connections (see listen())
    /// \return Socket intialised for the accepted connection from remote peer. After use,
    /// the user must delete the socket as follows
    /// \code
    /// TcpSocket* pSocket = accept();
    /// // use socket
    /// pSocket->close();
    /// delete pSocket;
    /// \endcode
    /// \see bind, listen
    TcpSocket* accept();

    unsigned int readn(std::vector<unsigned char>& buffer, unsigned int bytes);
    unsigned int write(const std::vector<unsigned char>& buffer);

}; // TcpSocket

} // Grape

#endif // GRAPE_TCPSOCKET_H
