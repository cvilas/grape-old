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
class GRAPEIO_DLL_API TcpSocket : public IpSocket
{
public:

    TcpSocket();
    virtual ~TcpSocket() throw(/*nothing*/);
    void close();

    /// Establish connection with remote peer
    /// \param peer Remote host
    /// \return true if connection was established successfully
    bool connect(struct sockaddr_in &peer);

    /// Enable TCP_NODELAY socket option (disable Nagle algorithm)
    /// \throw SocketException
    void setNoDelay(bool option);

    unsigned int send(const char *outMsgBuf, unsigned int outMsgLen);
    unsigned int receive(char *inMsgBuf, unsigned int inBufLen);

}; // TcpSocket

} // Grape

#endif // GRAPE_TCPSOCKET_H
