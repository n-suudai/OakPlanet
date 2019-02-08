
#include "Oak/Platform/NetWork/Win32/XSocket_Win32.hpp"
#include "Oak/Platform/OS/Win32.hpp"
#include <cstdio>


namespace Oak {


Socket::Socket()
    : m_isInitialized(false)
{
    ZeroMemory(&m_wsaData, sizeof(WSADATA));
    m_socket = INVALID_SOCKET;
}

Socket::~Socket()
{
    Terminate();
}

Bool Socket::Initialize(SOCKET_TYPE socketType)
{
    m_isInitialized = false;

    {
        int err;

        err = WSAStartup(WINSOCK_VERSION, &m_wsaData);

        if (err != 0)
        {
            /*
            WSASYSNOTREADY      ネットワークサブシステムがネットワークへの接続を準備できていない
            WSAVERNOTSUPPORTED  要求されたwinsockのバージョンがサポートされていない
            WSAEINPROGRESS      ブロッキング操作の実行中であるか、 またはサービスプロバイダがコールバック関数を処理している
            WSAEPROCLIM         winsockが処理できる最大プロセス数に達した
            WSAEFAULT           第二引数であるlpWSAData は有効なポインタではない
            */
            switch (err)
            {
            case WSASYSNOTREADY:
                OutputDebugStringA("WSASYSNOTREADY\n");
                break;
            case WSAVERNOTSUPPORTED:
                OutputDebugStringA("WSAVERNOTSUPPORTED\n");
                break;
            case WSAEINPROGRESS:
                OutputDebugStringA("WSAEINPROGRESS\n");
                break;
            case WSAEPROCLIM:
                OutputDebugStringA("WSAEPROCLIM\n");
                break;
            case WSAEFAULT:
                OutputDebugStringA("WSAEFAULT\n");
                break;
            }

            return false;
        }

        char buff[256];

        int versionHigh = static_cast<int>(static_cast<BYTE>(m_wsaData.wHighVersion));
        int versionLow = static_cast<int>(m_wsaData.wHighVersion >> 8);

        sprintf_s(
            buff,
            "winsock バージョン = %d.%d\n記述 = %s\n状態 = %s\n",
            versionHigh,
            versionLow,
            m_wsaData.szDescription,
            m_wsaData.szSystemStatus
        );
        OutputDebugStringA(buff);
    }

    int type = SOCK_STREAM;
    switch (socketType)
    {
    case SOCKET_TYPE_TCP:
        type = SOCK_STREAM;
        break;
    case SOCKET_TYPE_UDP:
        type = SOCK_DGRAM;
        break;
    default:
    {
        char buff[256];
        sprintf_s(
            buff,
            "socketType=%d is not supported.\n",
            static_cast<int>(socketType)
        );
        OutputDebugStringA(buff);
        return false;
    }
        break;
    }

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET)
    {
        OutputDebugStringA("socket() is failed.\n");
        return false;
    }

    m_isInitialized = true;

    return m_isInitialized;
}

Void Socket::Terminate()
{
    if (m_socket != INVALID_SOCKET)
    {
        shutdown(m_socket, SD_BOTH);
        closesocket(m_socket);
    }

    if (m_isInitialized)
    {
        WSACleanup();
        m_isInitialized = false;
    }
}


} // namespace Oak

