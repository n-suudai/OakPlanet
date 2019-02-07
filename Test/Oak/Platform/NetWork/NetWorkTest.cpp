
#include "NetWorkTest.hpp"
#include "Oak/Platform/NetWork.hpp"

#pragma comment(lib, "ws2_32.lib")


int NetWorkTestMain()
{
    Oak::Socket socket;

    socket.Initialize(Oak::SOCKET_TYPE_TCP);

    socket.Terminate();

    return 0;
}

