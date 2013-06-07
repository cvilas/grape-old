//==============================================================================
// Project  : Grape
// Module   : IO
// File     : UdpServer.h
// Brief    : UDP server class
//==============================================================================

#ifndef GRAPE_UDPSERVER_H
#define GRAPE_UDPSERVER_H

#include "UdpSocket.h"

namespace Grape
{

/// \brief UDP server
/// \ingroup io
///
/// This is nothing more than a UDP socket that binds to a port
class GRAPEIO_DLL_API UdpServer : public UdpSocket
{
public:
    UdpServer(unsigned int port) : UdpSocket()
    {
        allowPortReuse(true);
        bind(port);
    }
    ~UdpServer() throw() {}

};// UdpServer

} // Grape

#endif // GRAPE_UDPSERVER_H
