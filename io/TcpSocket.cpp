//==============================================================================
// Project  : Grape
// Module   : IO
// File     : TcpSocket.cpp
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

//--------------------------------------------------------------------------
static void throwSocketException(const std::string& location)
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


//==========================================================================
TcpSocket::TcpSocket()
//==========================================================================
{
#ifdef _MSC_VER
    WSADATA wsa_data;
    int err = WSAStartup (MAKEWORD(2,0), &wsa_data);
    if (err)
    {
        WSACleanup();
        throw WsaInitException(-1, "[TcpSocket::TcpSocket (WSAStartup)]");
    }
    if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 0)
    {
        WSACleanup();
        throw WsaInitException(-1, "[TcpSocket::TcpSocket]: Failed to load Windows Sockets 2.0");
    }
#endif

    // initialise socket
    if( (int)(_sockFd = socket(AF_INET /*ipv4*/, SOCK_STREAM /*TCP*/, IPPROTO_TCP)) == SOCKET_ERROR)
    {
        throwSocketException("[TcpSocket::TcpSocket](socket)");
    }

}

//--------------------------------------------------------------------------
TcpSocket::~TcpSocket()
//--------------------------------------------------------------------------
{
    close();

#ifdef _MSC_VER
    WSACleanup();
#endif
}

//--------------------------------------------------------------------------
void TcpSocket::close()
//--------------------------------------------------------------------------
{
    if( _sockFd != INVALID_SOCKET )
    {
        CLOSESOCKET(_sockFd);
        _sockFd = INVALID_SOCKET;
    }
}

//--------------------------------------------------------------------------
void TcpSocket::allowPortReuse(bool yes)
//--------------------------------------------------------------------------
{
    if( setsockopt(_sockFd, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(int)) == SOCKET_ERROR)
    {
        throwSocketException("[TcpSocket::allowPortReuse](setsockopt)");
    }
}

//--------------------------------------------------------------------------
void TcpSocket::setBufSize(unsigned int sz)
//--------------------------------------------------------------------------
{
    if( setsockopt(_sockFd, SOL_SOCKET, SO_SNDBUF, (char *)&sz, sizeof(unsigned int)) == SOCKET_ERROR)
    {
        throwSocketException("[TcpSocket::setBufSize(SO_SNDBUF)]");
    }

    if( setsockopt(_sockFd, SOL_SOCKET, SO_RCVBUF, (char *)&sz, sizeof(unsigned int)) == SOCKET_ERROR)
    {
        throwSocketException("[TcpSocket::setBufSize(SO_RCVBUF)]");
    }
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
void TcpSocket::bind(int port)
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
bool TcpSocket::connect(struct sockaddr_in &peer)
//--------------------------------------------------------------------------
{
    return (::connect(_sockFd, (struct sockaddr*)(&peer), sizeof(struct sockaddr)) != SOCKET_ERROR );
}

//--------------------------------------------------------------------------
unsigned int TcpSocket::send(const char *outMsgBuf, unsigned int outMsgLen)
//--------------------------------------------------------------------------
{
    if( INVALID_SOCKET == _sockFd )
    {
        throw SocketException(-1, "[TcpSocket::send] Socket not initialised");
    }

    int len = ::send(_sockFd, outMsgBuf, outMsgLen, 0);
    if( len == SOCKET_ERROR )
    {
        throwSocketException("[TcpSocket::send(send)]");
    }

    return len;
}

//--------------------------------------------------------------------------
unsigned int TcpSocket::receive(char *inMsgBuf, unsigned int inBufLen)
//--------------------------------------------------------------------------
{
    if( INVALID_SOCKET == _sockFd )
    {
        throw SocketException(-1, "[TcpSocket::receive] Socket not initialised");
    }

    // wait for messages
    int len = ::recv(_sockFd, inMsgBuf, inBufLen, 0/*MSG_WAITALL*/);
    if( len == SOCKET_ERROR )
    {
        throwSocketException("[TcpSocket::receive(recv)]");
    }

    return len;
}

//--------------------------------------------------------------------------
void TcpSocket::setRecvTimeout(unsigned long int ms)
//--------------------------------------------------------------------------
{
    // set receive timeout
    int ret = 0;
#ifdef _MSC_VER
    ret = setsockopt(sockFd_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&ms, sizeof(unsigned long int));
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

} // Grape
