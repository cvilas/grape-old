//==============================================================================
// Project  : Grape
// Module   : IO
// File     : TcpSocket.cpp
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

#include "TcpSocket.h"

#ifndef _MSC_VER // UNIX platforms?
#define CLOSESOCKET(fd) ::close(fd)
#define SOCKET_ERROR -1
#define INVALID_SOCKET (SOCKET)(-1)
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#ifdef _MSC_VER
#define CLOSESOCKET(fd) closesocket(fd)
#define snprintf _snprintf
#endif

#include <iostream>
#include <sstream>
#include <errno.h>

namespace Grape
{

//==========================================================================
TcpSocket::TcpSocket()
//==========================================================================
    : IpSocket()
{
    if( (int)(_sockFd = socket(AF_INET /*ipv4*/, SOCK_STREAM /*TCP*/, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        throwSocketException("[TcpSocket::TcpSocket](socket)");
    }

}

//--------------------------------------------------------------------------
TcpSocket::~TcpSocket() throw()
//--------------------------------------------------------------------------
{
}

//--------------------------------------------------------------------------
bool TcpSocket::connect(const std::string& remoteIp, int remotePort)
//--------------------------------------------------------------------------
{
    struct sockaddr_in peer = getSocketAddress(remoteIp, remotePort);
    return (::connect(_sockFd, (struct sockaddr*)(&peer), sizeof(struct sockaddr)) != SOCKET_ERROR );
}

//--------------------------------------------------------------------------
void TcpSocket::listen(int backlog)
//--------------------------------------------------------------------------
{
    if( ::listen(_sockFd, backlog) == SOCKET_ERROR )
    {
        throwSocketException("[TcpSocket::listen]");
    }
}

//--------------------------------------------------------------------------
TcpSocket* TcpSocket::accept()
//--------------------------------------------------------------------------
{
    TcpSocket* client = new TcpSocket;
    SOCKET fd = ::accept(_sockFd, NULL, NULL);
    if( fd == INVALID_SOCKET )
    {
        throwSocketException("[TcpSocket::accept]");
    }
    client->setSockFd(fd);
    return client;
}

//--------------------------------------------------------------------------
void TcpSocket::setNoDelay(bool yes)
//--------------------------------------------------------------------------
{
    int val = (yes?1:0);
    if( setsockopt(_sockFd, IPPROTO_TCP, TCP_NODELAY, (const char *)&val, sizeof(int)) == SOCKET_ERROR)
    {
        throwSocketException("[TcpSocket::setNoDelay(TCP_NODELAY)]");
    }
}

//--------------------------------------------------------------------------
unsigned int TcpSocket::write(const std::vector<unsigned char>& buffer)
//--------------------------------------------------------------------------
{
    int len = ::send(_sockFd, (const char*)&buffer[0], buffer.size(), 0);

    if( len == SOCKET_ERROR )
    {
        throwSocketException("[TcpSocket::send(send)]");
    }

    return len;
}

//--------------------------------------------------------------------------
unsigned int TcpSocket::readn(std::vector<unsigned char>& buffer, unsigned int bytesToRead)
//--------------------------------------------------------------------------
{
    if( bytesToRead > buffer.size() )
    {
        buffer.resize(bytesToRead);
    }

    int len = ::recv(_sockFd, (char*)(&buffer[0]), bytesToRead, 0/*MSG_WAITALL*/);
    if( len == SOCKET_ERROR )
    {
        throwSocketException("[TcpSocket::receive(recv)]");
    }

    return len;
}


} // Grape
