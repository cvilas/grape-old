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
class GRAPEIO_DLL_API UdpSocket : public IpSocket
{
public:

    UdpSocket();
    virtual ~UdpSocket() throw(/*nothing*/);

    /// Specify remote host. (This doesn't actually cause a connection to be
    /// established, as UDP communication is connectionless. The method copies
    /// peer information, and uses them in send() and receive()
    /// \return true (always)
    bool setRemotePeer(struct sockaddr_in &peer);

    /// \copydoc IDataPort::readn()
    /// Since UDP sockets are connectionless, this method will read data from any remote host.
    /// To get data source information, use readFrom()
    unsigned int readn(std::vector<unsigned char>& buffer, unsigned int bytes);

    /// \copydoc IDataPort::write()
    /// This method writes to remote peer specified in setRemotePeer()
    unsigned int write(const std::vector<unsigned char>& buffer);

    /// Send message to a specific host
    /// \param destAddr   Destination address information
    /// \param buffer  Pointer to buffer containing your message.
    /// \throw SocketException
    /// \return number of bytes sent
    unsigned int writeTo(struct sockaddr_in &destAddr, const std::vector<unsigned char>& buffer);

    /// Block to receive message from any host
    /// \param buffer Buffer to receive message into
    /// \param srcAddr The address of the host from which data was received.
    /// \throw SocketException
    /// \return number of bytes received
    unsigned int readFrom(std::vector<unsigned char>& buffer, struct sockaddr_in &srcAddr);

private:
    sockaddr_in _peer;

};// UdpSocket

} // Grape

#endif // GRAPE_UDPSOCKET_H
