//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IpSocket.cpp
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
IpSocket::IpSocket()
//==========================================================================
    : _sockFd(INVALID_SOCKET)
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
IpSocket::~IpSocket() throw()
//--------------------------------------------------------------------------
{
    close();

#ifdef _MSC_VER
    WSACleanup();
#endif
}

//--------------------------------------------------------------------------
void IpSocket::close()
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
        throwSocketException("[TcpSocket::bind]");
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
        throwSocketException("[TcpSocket::setRecvTimeout]");
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

} // Grape
