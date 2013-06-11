//==============================================================================
// Project  : Grape
// Module   : IO
// File     : UdpSocket.cpp
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

namespace Grape
{

//==========================================================================
UdpSocket::UdpSocket()
//==========================================================================
{
    if( (int)(_sockFd = socket(AF_INET /*ipv4*/, SOCK_DGRAM /*UDP*/, IPPROTO_UDP)) == SOCKET_ERROR)
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
bool UdpSocket::connect(struct sockaddr_in &peer)
//--------------------------------------------------------------------------
{
    memcpy(&_peer, &peer, sizeof(peer));
    return true;
}

//--------------------------------------------------------------------------
unsigned int UdpSocket::sendTo(struct sockaddr_in &destAddr, const char *outMsgBuf, unsigned int outMsgLen)
//--------------------------------------------------------------------------
{
    if( INVALID_SOCKET == _sockFd )
    {
        throw SocketException(-1, "[UdpSocket::sendTo] Socket not initialised");
    }

    int len = sendto(_sockFd, outMsgBuf, outMsgLen, 0, (struct sockaddr *)&destAddr, sizeof(struct sockaddr_in));
    if( len == SOCKET_ERROR )
    {
        throwSocketException("[UdpSocket::sendTo(sendto)]");
    }

    return len;
}

//--------------------------------------------------------------------------
unsigned int UdpSocket::receiveFrom(char *inMsgBuf, unsigned int inBufLen, struct sockaddr_in &srcAddr)
//--------------------------------------------------------------------------
{
    if( INVALID_SOCKET == _sockFd )
    {
        throw SocketException(-1, "[UdpSocket::receiveFrom] Socket not initialised");
    }

    int srcAddrLen = sizeof(struct sockaddr);

    // wait for messages
    int len = ::recvfrom(_sockFd, inMsgBuf, inBufLen, 0/*MSG_WAITALL*/, (struct sockaddr *) &(srcAddr), (socklen_t *)&srcAddrLen);
    if( len == SOCKET_ERROR )
    {
        throwSocketException("[UdpSocket::receiveFrom(receivefrom)]");
    }

    return len;
}

//--------------------------------------------------------------------------
unsigned int UdpSocket::send(const char *outMsgBuf, unsigned int outMsgLen)
//--------------------------------------------------------------------------
{
    if( INVALID_SOCKET == _sockFd )
    {
        throw SocketException(-1, "[UdpSocket::send] Socket not initialised");
    }

    int len = sendto(_sockFd, outMsgBuf, outMsgLen, 0, (struct sockaddr *)&_peer, sizeof(struct sockaddr_in));
    if( len == SOCKET_ERROR )
    {
        throwSocketException("[UdpSocket::send(sendto)]");
    }

    return len;
}

//--------------------------------------------------------------------------
unsigned int UdpSocket::receive(char *inMsgBuf, unsigned int inBufLen)
//--------------------------------------------------------------------------
{
    if( INVALID_SOCKET == _sockFd )
    {
        throw SocketException(-1, "[UdpSocket::receive] Socket not initialised");
    }

    struct sockaddr_in srcAddr;
    int srcAddrLen = sizeof(struct sockaddr);

    // wait for messages
    int len = ::recvfrom(_sockFd, inMsgBuf, inBufLen, 0/*MSG_WAITALL*/, (struct sockaddr *) &(srcAddr), (socklen_t *)&srcAddrLen);

    // TODO:
    // - check srcAddr against _peer. call not the same, go back and call recvfrom again

    if( len == SOCKET_ERROR )
    {
        throwSocketException("[UdpSocket::receive(recvfrom)]");
    }

    return len;
}


} // Grape
