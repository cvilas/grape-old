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

    /// Establish connection with remote peer
    /// \param remoteIp Remote host IP
    /// \param remotePort Port to connect to on remote host
    /// \return true if connection was established successfully
    bool connect(const std::string& remoteIp,int remotePort);

    /// Enable TCP_NODELAY socket option (disable Nagle algorithm)
    /// \throw SocketException
    void setNoDelay(bool option);

    unsigned int readn(std::vector<unsigned char>& buffer, unsigned int bytes);
    unsigned int write(const std::vector<unsigned char>& buffer);

}; // TcpSocket

} // Grape

#endif // GRAPE_TCPSOCKET_H
