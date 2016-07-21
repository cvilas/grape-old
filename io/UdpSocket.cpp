//==============================================================================
// Project  : Grape
// Module   : IO
// File     : UdpSocket.cpp
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

#include "UdpSocket.h"

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

namespace grape
{

//==========================================================================
UdpSocket::UdpSocket()
//==========================================================================
{
    if( (int)(_sockFd = socket(AF_INET /*ipv4*/, SOCK_DGRAM /*UDP*/, IPPROTO_UDP)) == INVALID_SOCKET)
    {
        throwSocketException("[UdpSocket::UdpSocket](socket)");
    }
}

//--------------------------------------------------------------------------
UdpSocket::~UdpSocket() throw()
//--------------------------------------------------------------------------
{
}

//--------------------------------------------------------------------------
bool UdpSocket::setRemotePeer(struct sockaddr_in &peer)
//--------------------------------------------------------------------------
{
    memcpy(&_peer, &peer, sizeof(peer));
    return true;
}

//--------------------------------------------------------------------------
unsigned int UdpSocket::writeTo(struct sockaddr_in &destAddr, const std::vector<unsigned char>& buffer)
//--------------------------------------------------------------------------
{
    int len = sendto(_sockFd, (const char*)&buffer[0], buffer.size(), 0, (struct sockaddr *)&destAddr, sizeof(struct sockaddr_in));
    if( len == SOCKET_ERROR )
    {
        throwSocketException("[UdpSocket::writeTo(sendto)]");
    }

    return len;
}

//--------------------------------------------------------------------------
unsigned int UdpSocket::readFrom(std::vector<unsigned char>& buffer, struct sockaddr_in &srcAddr)
//--------------------------------------------------------------------------
{
    int srcAddrLen = sizeof(struct sockaddr);

    int len = ::recvfrom(_sockFd, (char*)(&buffer[0]), buffer.size(), 0/*MSG_WAITALL*/, (struct sockaddr *) &(srcAddr), (socklen_t *)&srcAddrLen);
    if( len == SOCKET_ERROR )
    {
        throwSocketException("[UdpSocket::readFrom(receivefrom)]");
    }

    return len;
}

//--------------------------------------------------------------------------
unsigned int UdpSocket::write(const std::vector<unsigned char>& buffer)
//--------------------------------------------------------------------------
{
    int len = sendto(_sockFd, (const char*)&buffer[0], buffer.size(), 0, (struct sockaddr *)&_peer, sizeof(struct sockaddr_in));
    if( len == SOCKET_ERROR )
    {
        throwSocketException("[UdpSocket::write(sendto)]");
    }

    return len;
}

//--------------------------------------------------------------------------
unsigned int UdpSocket::readn(std::vector<unsigned char>& buffer, unsigned int bytesToRead)
//--------------------------------------------------------------------------
{
    if( bytesToRead > buffer.size() )
    {
        buffer.resize(bytesToRead);
    }

    struct sockaddr_in srcAddr;
    int srcAddrLen = sizeof(struct sockaddr);

    // wait for messages
    int len = ::recvfrom(_sockFd, (char*)&buffer[0], bytesToRead, 0/*MSG_WAITALL*/, (struct sockaddr *) &(srcAddr), (socklen_t *)&srcAddrLen);

    if( len == SOCKET_ERROR )
    {
        throwSocketException("[UdpSocket::receive(recvfrom)]");
    }

    return len;
}


} // grape
