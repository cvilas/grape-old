//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IpClient.h
// Brief    : Base interface for IP clients
//==============================================================================

#ifndef GRAPEIO_IPCLIENT_H
#define GRAPEIO_IPCLIENT_H

#ifdef _MSC_VER // using microsoft visual studio
#include <Winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
typedef u_int in_addr_t;
#else // UNIX platforms?
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "SocketException.h"
#include <string>

namespace Grape
{

/// \class IpClient
/// \ingroup io
/// \brief Base class interface for IP clients
class GRAPEIO_DLL_API IpClient
{
public:

    /// Constructor. Keeps remote host connection parameters, but doesn't actually
    /// connect (this is left to derived classes)
    /// \param serverIp Remote host IP
    /// \param port Port to connect to on remote host
    /// \param msTimeout Receive timeout in ms
    /// \param bufSizeBytes send and receive buffer sizes in bytes
    /// \throw WsaInitException, HostInfoException
    IpClient(const std::string& serverIp, int port);

    virtual ~IpClient();

    /// Send message to server
    /// \param outMsgBuf  Pointer to buffer containing your message to server.
    /// \param outMsgLen  The length of your message above.
    /// \throw SocketException
    /// \return false on error
    virtual bool send(const unsigned char *outMsgBuf, unsigned int outMsgLen) = 0;

    /// block to receive message from server or timeout
    /// \param inMsgBuf Buffer to receive message into
    /// \param inBufLen The size (bytes) of the above buffer.
    /// \param inMsgLen  The actual length (bytes) of message received
    /// \param ms        timeout (ms)
    /// \throw SocketException
    /// \return false on error
    virtual bool receive(unsigned char *inMsgBuf, unsigned int inBufLen, unsigned int &inMsgLen) = 0;

protected:
    struct sockaddr_in _serverEndpoint;

}; // IpClient

} // Grape

#endif // GRAPEIO_IPCLIENT_H
