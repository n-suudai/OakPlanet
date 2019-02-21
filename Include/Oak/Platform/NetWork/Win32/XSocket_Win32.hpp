#pragma once

#include "Oak/Platform/NetWork/Interface/ISocket.hpp"
#include <winsock2.h>

namespace Oak
{

class Socket final : public Detail::ISocket
{
public:
    Socket();

    ~Socket();

    Bool Initialize(SOCKET_TYPE socketType) override;

    Void Terminate() override;

private:
    Bool m_isInitialized;
    WSADATA m_wsaData;
    SOCKET m_socket;
};

} // namespace Oak
