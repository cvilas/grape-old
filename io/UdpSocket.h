//==============================================================================
// Project  : Grape
// Module   : IO
// File     : UdpSocket.h
// Brief    : UDP socket class
//==============================================================================

#ifndef GRAPE_UDPSOCKET_H
#define GRAPE_UDPSOCKET_H

#include "IpSocket.h"

namespace Grape
{

/// \brief UDP socket
/// \ingroup io
///
/// Note that any method can throw SocketException on error
/// \todo
/// - Implement receive() properly.
class GRAPEIO_DLL_API UdpSocket : public IpSocket
{
public:

    UdpSocket();
    virtual ~UdpSocket() throw(/*nothing*/);

    /// Specify remote host. (This doesn't actually cause a connection to be
    /// established, as UDP communication is connectionless. The method copies
    /// peer information, and uses them in send() and receive()
    /// \return true (always)
    bool connect(struct sockaddr_in &peer);

    unsigned int send(const char *outMsgBuf, unsigned int outMsgLen);

    unsigned int receive(char *inMsgBuf, unsigned int inBufLen);

    /// Send message to a specific host
    /// \param destAddr   Destination address information
    /// \param outMsgBuf  Pointer to buffer containing your message to server.
    /// \param outMsgLen  The length of your message above.
    /// \throw SocketException
    /// \return number of bytes sent
    unsigned int sendTo(struct sockaddr_in &destAddr, const char *outMsgBuf, unsigned int outMsgLen);

    /// Block to receive message from any host or timeout
    /// \param inMsgBuf Buffer to receive message into
    /// \param inBufLen The size (bytes) of the above buffer.
    /// \param srcAddr The address of the host from which data was received.
    /// \throw SocketException
    /// \return number of bytes received
    unsigned int receiveFrom(char *inMsgBuf, unsigned int inBufLen, struct sockaddr_in &srcAddr);

private:
    sockaddr_in _peer;

};// UdpSocket

} // Grape

#endif // GRAPE_UDPSOCKET_H
