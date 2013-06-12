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
bool TcpSocket::connect(const std::string& remoteIp, int remotePort)
//--------------------------------------------------------------------------
{
    struct sockaddr_in peer = getSocketAddress(remoteIp, remotePort);
    return (::connect(_sockFd, (struct sockaddr*)(&peer), sizeof(struct sockaddr)) != SOCKET_ERROR );
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
