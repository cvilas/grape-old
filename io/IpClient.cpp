//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IpClient.cpp
//==============================================================================

#include "IpClient.h"
#ifdef _MSC_VER // using microsoft visual studio
#else
#include <sys/types.h>
#include <netdb.h>
#endif
#include <sstream>

namespace Grape
{

//==========================================================================
IpClient::IpClient(const std::string& serverIp, int port)
//==========================================================================
{
#ifdef _MSC_VER
    WSADATA wsa_data;
    int err = WSAStartup (MAKEWORD(2,0), &wsa_data);
    if (err)
    {
        WSACleanup();
        throw WsaInitException(-1, "[IpClient::IpClient (WSAStartup)]");
    }
    if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 0)
    {
        WSACleanup();
        throw WsaInitException(-1, "[IpClient::IpClient]: Failed to load Windows Sockets 2.0");
    }
#endif

    // get network server entry
    struct hostent* pHost = NULL;
    pHost = gethostbyname(serverIp.c_str());
    if( pHost == NULL )
    {
#ifdef _MSC_VER
        throw SocketException(WSAGetLastError(), "[IpClient::init(gethostbyname)]");
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
}

//--------------------------------------------------------------------------
IpClient::~IpClient()
//--------------------------------------------------------------------------
{
#ifdef _MSC_VER
    WSACleanup();
#endif
}


} // Grape
