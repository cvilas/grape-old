//==============================================================================
// Project  : Grape
// Module   : IO
// File     : TcpSocket.h
// Brief    : TCP socket class
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
