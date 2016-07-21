//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IpSocket.cpp
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

#include "IpSocket.h"

#ifndef _MSC_VER // UNIX platforms?
#define CLOSESOCKET(fd) ::close(fd)
#define SOCKET_ERROR -1
#define INVALID_SOCKET (SOCKET)(-1)
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/ioctl.h>
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
IpSocket::IpSocket()
//==========================================================================
    : IDataPort(), _sockFd(INVALID_SOCKET)
{
#ifdef _MSC_VER
    WSADATA wsa_data;
    int err = WSAStartup (MAKEWORD(2,0), &wsa_data);
    if (err)
    {
        WSACleanup();
        throw WsaInitException(-1, "[IpSocket::IpSocket (WSAStartup)]");
    }
    if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 0)
    {
        WSACleanup();
        throw WsaInitException(-1, "[IpSocket::IpSocket]: Failed to load Windows Sockets 2.0");
    }
#endif
}

//--------------------------------------------------------------------------
struct sockaddr_in IpSocket::getSocketAddress(const std::string& serverIp, int port)
//--------------------------------------------------------------------------
{
    // get network server entry
    struct hostent* pHost = NULL;
    pHost = gethostbyname(serverIp.c_str());
    if( pHost == NULL )
    {
#ifdef _MSC_VER
        throw HostInfoException(WSAGetLastError(), "[IpClient::init(gethostbyname)]");
#else
        std::ostringstream str;
        str << "[IpSocket::IpClient(gethostbyname)]: " << hstrerror(h_errno);
        throw HostInfoException(h_errno, str.str());
#endif
    }

    // server socket info
    struct sockaddr_in remoteEndpoint;
    remoteEndpoint.sin_family = AF_INET;
    remoteEndpoint.sin_port = htons(port);
    remoteEndpoint.sin_addr.s_addr = *((in_addr_t *)pHost->h_addr);
    memset(&(remoteEndpoint.sin_zero), '\0', 8);

    return remoteEndpoint;
}

//--------------------------------------------------------------------------
std::string IpSocket::getHostName()
//--------------------------------------------------------------------------
{
    sockaddr_in addr;
    socklen_t len = sizeof(sockaddr_in);
    if( getsockname(_sockFd, (sockaddr*)&addr, &len) == SOCKET_ERROR )
    {
        throwSocketException("[IpSocket::getSocketName]");
    }
    std::ostringstream str;
    str << inet_ntoa(addr.sin_addr) << ":" << addr.sin_port;
    return str.str();
}

//--------------------------------------------------------------------------
std::string IpSocket::getPeerName()
//--------------------------------------------------------------------------
{
    sockaddr_in addr;
    socklen_t len = sizeof(sockaddr_in);
    if( getpeername(_sockFd, (sockaddr*)&addr, &len) == SOCKET_ERROR )
    {
        throwSocketException("[IpSocket::getPeerName]");
    }
    std::ostringstream str;
    str << inet_ntoa(addr.sin_addr) << ":" << addr.sin_port;
    return str.str();
}

//--------------------------------------------------------------------------
IpSocket::~IpSocket() throw()
//--------------------------------------------------------------------------
{
    close();

#ifdef _MSC_VER
    WSACleanup();
#endif
}

//--------------------------------------------------------------------------
void IpSocket::close() throw()
//--------------------------------------------------------------------------
{
    if( _sockFd != INVALID_SOCKET )
    {
        CLOSESOCKET(_sockFd);
        _sockFd = INVALID_SOCKET;
    }
}

//--------------------------------------------------------------------------
void IpSocket::allowPortReuse(bool yes)
//--------------------------------------------------------------------------
{
    if( setsockopt(_sockFd, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(int)) == SOCKET_ERROR)
    {
        throwSocketException("[IpSocket::allowPortReuse](setsockopt)");
    }
}

//--------------------------------------------------------------------------
void IpSocket::setBufSize(unsigned int sz)
//--------------------------------------------------------------------------
{
    if( setsockopt(_sockFd, SOL_SOCKET, SO_SNDBUF, (char *)&sz, sizeof(unsigned int)) == SOCKET_ERROR)
    {
        throwSocketException("[IpSocket::setBufSize(SO_SNDBUF)]");
    }

    if( setsockopt(_sockFd, SOL_SOCKET, SO_RCVBUF, (char *)&sz, sizeof(unsigned int)) == SOCKET_ERROR)
    {
        throwSocketException("[IpSocket::setBufSize(SO_RCVBUF)]");
    }
}

//--------------------------------------------------------------------------
void IpSocket::bind(int port)
//--------------------------------------------------------------------------
{
    struct sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&(name.sin_zero), '\0', 8);
    if( ::bind(_sockFd, (struct sockaddr *)&name, sizeof(struct sockaddr)) == SOCKET_ERROR)
    {
        throwSocketException("[IpSocket::bind]");
    }
}

//--------------------------------------------------------------------------
void IpSocket::setRecvTimeout(unsigned long int ms)
//--------------------------------------------------------------------------
{
    // set receive timeout
    int ret = 0;
#ifdef _MSC_VER
    ret = setsockopt(_sockFd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&ms, sizeof(unsigned long int));
#else
    struct timeval tmo;
    tmo.tv_sec = ms/1000UL;
    tmo.tv_usec = (ms%1000UL) * 1000000UL;
    ret = setsockopt(_sockFd, SOL_SOCKET, SO_RCVTIMEO, &tmo, sizeof(struct timeval));
#endif
    if( ret == SOCKET_ERROR )
    {
        throwSocketException("[IpSocket::setRecvTimeout]");
    }
}

//--------------------------------------------------------------------------
void IpSocket::throwSocketException(const std::string& location)
//--------------------------------------------------------------------------
{
    int e;
    std::ostringstream str;
#ifdef _MSC_VER
    e = WSAGetLastError();
    str << location << ": Got WSA error " << e;
#else
    e = errno;
    str << location << ": " << strerror(e);
#endif
    throw SocketException(e, str.str());
}

//------------------------------------------------------------------------------
unsigned int IpSocket::availableToRead()
//------------------------------------------------------------------------------
{
    unsigned int bytes = 0;
#ifdef _MSC_VER
    if( ioctlsocket(_sockFd, FIONREAD, (unsigned long*)&bytes) == SOCKET_ERROR )
#else
    if( ioctl(_sockFd, FIONREAD, &bytes) < 0 )
#endif
    {
        throwSocketException("[IpSocket::availableToRead]");
    }

    return bytes;
}

//--------------------------------------------------------------------------
IDataPort::Status IpSocket::waitForRead(int timeoutMs)
//--------------------------------------------------------------------------
{
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(_sockFd, &fds);

    // indefinite wait
    int ret = 0;
    if( timeoutMs < 0 )
    {
        ret = select(_sockFd+1, &fds, NULL, NULL, NULL);
    }

    // wait with timeout
    else
    {
        long long int secs = timeoutMs/1000;
        long long int usecs = (timeoutMs * 1000) - (secs * 1000000);
        struct timeval timeout = {secs, usecs};
        ret = select(_sockFd+1, &fds, NULL, NULL, &timeout);
    }

    IDataPort::Status st = IDataPort::PORT_ERROR;
    // ret == 0: timeout, ret == 1: ready, ret == -1: error
    if (ret > 0)
    {
        // linux workaround. select returns 1 for closed socket, but there
        // is nothing to read
        if( availableToRead() )
        {
            st = IDataPort::PORT_OK;
        }
    }
    else if (ret == 0)
    {
        st = IDataPort::PORT_TIMEOUT;
    }
    else
    {
        throw IoEventHandlingException(errno, "[IpSocket::waitForRead(select)] failed");
    }

    return st;
}


} // grape
