//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IpClient.h
// Brief    : IP client interface
//==============================================================================

#ifndef GRAPEIO_IPCLIENT_H
#define GRAPEIO_IPCLIENT_H

#include "IpSocket.h"
#include <string>

namespace Grape
{

/// \class IpClient
/// \ingroup io
/// \brief IP client interface. Use this to connect to a remote TCP or UDP server
class GRAPEIO_DLL_API IpClient
{
public:

    /// Initialise and connect to a remote host
    /// \param serverIp Remote host IP
    /// \param port Port to connect to on remote host
    /// \param msTimeout Receive timeout in ms
    /// \param bufSizeBytes send and receive buffer sizes in bytes
    /// \throw WsaInitException, HostInfoException
    IpClient(IpSocket::SocketType type, const std::string& serverIp, int port);

    ~IpClient();

    /// Send message to remote host
    /// \param outMsgBuf  Pointer to buffer containing your message to server.
    /// \param outMsgLen  The length of your message above.
    /// \throw SocketException
    /// \return number of bytes sent
    unsigned int send(const unsigned char *outMsgBuf, unsigned int outMsgLen);

    /// Block to receive message from remote host or timeout
    /// \param inMsgBuf Buffer to receive message into
    /// \param inBufLen The size (bytes) of the above buffer.
    /// \param ms        timeout (ms)
    /// \throw SocketException
    /// \return number of bytes received
    unsigned int receive(unsigned char *inMsgBuf, unsigned int inBufLen);

protected:
    struct sockaddr_in _serverEndpoint;
    IpSocket* _pSocket;

}; // IpClient



} // Grape

#endif // GRAPEIO_IPCLIENT_H
