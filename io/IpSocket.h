//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IpSocket.h
// Brief    : Base class for IP socket
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

#include "IDataPort.h"
#include "SocketException.h"

namespace Grape
{

/// \brief Base class for BSD sockets
/// \ingroup io
///
/// See derived classes for specific implementations. Socket classes implement the
/// IDataPort interface
///
/// Example Programs:
/// See TcpSocket
class GRAPEIO_DLL_API IpSocket : public IDataPort
{
public:

    /// Base class contructor initialises socket library. Derived classes
    /// initialise socket.
    /// \throw WsaInitException, SocketException
    IpSocket();

    // ------------- Reimplemented from IDataPort -------------------

    virtual ~IpSocket() throw(/*nothing*/);
    virtual void close() throw(/*nothing*/);
    unsigned int availableToRead();
    unsigned int readAll(std::vector<unsigned char>& buffer) { return readn(buffer, availableToRead()); }
    IDataPort::Status waitForRead(int timeoutMs);
    IDataPort::Status waitForWrite(int timeoutMs) { return IDataPort::PORT_OK; } //!< does nothing
    void flushRx() {} //!< does nothing
    void flushTx() {} //!< does nothing

    // ------------- Socket specific methods -------------------

    /// bind socket to a port number
    /// \see listen, accept
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

    /// \return the current address to which the socket is bound, in
    /// the format ip:port
    std::string getHostName();

    /// \return The remote endpoint for the connected socket, in
    /// the format ip:port
    std::string getPeerName();

protected:

    /// Construct a socket address for a remote endpoint
    /// \param remoteIp Remote host IP
    /// \param remotePort Port to connect to on remote host
    struct sockaddr_in getSocketAddress(const std::string& remoteIp, int remotePort);

    /// Set the underlying socket file descriptor. Use with caution, as there
    /// are not error checks
    void setSockFd(SOCKET fd) { _sockFd = fd; }

    /// \return BSD socket file descriptor
    SOCKET getSockFd() const { return _sockFd; }

    /// Throw a SocketException. Specify location from where it was thrown in
    /// order to help the user
    void throwSocketException(const std::string& location);

protected:
    SOCKET _sockFd;

}; // IpSocket

} // Grape

#endif // GRAPE_IPSOCKET_H
