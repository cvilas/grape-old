//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IpClient.cpp
//==============================================================================

#include "IpClient.h"
#include "TcpSocket.h"
#include "UdpSocket.h"
#ifndef _MSC_VER // UNIX platforms?
#include <netdb.h>
#endif
#include <sstream>

namespace Grape
{

//==========================================================================
IpClient::IpClient(IpSocket::SocketType type, const std::string& serverIp, int port)
//==========================================================================
    : _pSocket(NULL)
{
    switch(type)
    {
    case IpSocket::TCP:
        _pSocket = new TcpSocket;
        break;
    case IpSocket::UDP:
        _pSocket = new UdpSocket;
        break;
    };

    // get network server entry
    struct hostent* pHost = NULL;
    pHost = gethostbyname(serverIp.c_str());
    if( pHost == NULL )
    {
#ifdef _MSC_VER
        throw HostInfoException(WSAGetLastError(), "[IpClient::init(gethostbyname)]");
#else
        std::ostringstream str;
        str << "[IpClient::IpClient(gethostbyname)]: " << hstrerror(h_errno);
        throw HostInfoException(h_errno, str.str());
#endif
    }

    // server socket info
    struct sockaddr_in remoteEndpoint;
    remoteEndpoint.sin_family = AF_INET;
    remoteEndpoint.sin_port = htons(port);
    remoteEndpoint.sin_addr.s_addr = *((in_addr_t *)pHost->h_addr);
    memset(&(remoteEndpoint.sin_zero), '\0', 8);

    _pSocket->connect(remoteEndpoint);
    //_socket->bind(INADDR_ANY);
}

//--------------------------------------------------------------------------
IpClient::~IpClient()
//--------------------------------------------------------------------------
{
    delete _pSocket;
}

//--------------------------------------------------------------------------
unsigned int IpClient::send(const char *outMsgBuf, unsigned int outMsgLen)
//--------------------------------------------------------------------------
{
    return _pSocket->send(outMsgBuf, outMsgLen);
}

//--------------------------------------------------------------------------
unsigned int IpClient::receive(char *inMsgBuf, unsigned int inBufLen)
//--------------------------------------------------------------------------
{
    return _pSocket->receive(inMsgBuf, inBufLen);
}

} // Grape
