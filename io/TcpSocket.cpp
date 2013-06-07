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

//==========================================================================
TcpSocket::TcpSocket()
//==========================================================================
    : IpSocket()
{
    if( (int)(_sockFd = socket(AF_INET /*ipv4*/, SOCK_STREAM /*TCP*/, IPPROTO_TCP)) == SOCKET_ERROR)
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

} // Grape
