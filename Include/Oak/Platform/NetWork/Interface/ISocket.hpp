#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"


namespace Oak {


enum SOCKET_TYPE
{
    SOCKET_TYPE_TCP,
    SOCKET_TYPE_UDP,
    SOCKET_TYPE_NUM,
};


namespace Detail {


class ISocket
{
protected:
    ISocket() = default;

    virtual ~ISocket() = default;

public:
    virtual Bool Initialize(SOCKET_TYPE socketType) = 0;

    virtual Void Terminate() = 0;

};


} // namespace Detail
} // namespace Oak

