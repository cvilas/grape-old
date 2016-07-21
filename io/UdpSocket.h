//==============================================================================
// Project  : Grape
// Module   : IO
// File     : UdpSocket.h
// Brief    : UDP socket class
//
// Copyright (c) 2012, Vilas Chitrakaran
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================

#ifndef GRAPE_UDPSOCKET_H
#define GRAPE_UDPSOCKET_H

#include "IpSocket.h"

namespace grape
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

} // grape

#endif // GRAPE_UDPSOCKET_H
