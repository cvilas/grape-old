//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IpClient.cpp
//==============================================================================

#include "IpClient.h"
#include "TcpSocket.h"
#include "UdpSocket.h"
#include <netdb.h>
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
    _serverEndpoint.sin_family = AF_INET;
    _serverEndpoint.sin_port = htons(port);
    _serverEndpoint.sin_addr.s_addr = *((in_addr_t *)pHost->h_addr);
    memset(&(_serverEndpoint.sin_zero), '\0', 8);

    _pSocket->connect(_serverEndpoint);
    //_socket->bind(INADDR_ANY);
}

//--------------------------------------------------------------------------
IpClient::~IpClient()
//--------------------------------------------------------------------------
{
    delete _pSocket;
}

//--------------------------------------------------------------------------
unsigned int IpClient::send(const unsigned char *outMsgBuf, unsigned int outMsgLen)
//--------------------------------------------------------------------------
{
    return _pSocket->send(outMsgBuf, outMsgLen);
}

//--------------------------------------------------------------------------
unsigned int IpClient::receive(unsigned char *inMsgBuf, unsigned int inBufLen)
//--------------------------------------------------------------------------
{
    return _pSocket->receive(inMsgBuf, inBufLen);
}

} // Grape
