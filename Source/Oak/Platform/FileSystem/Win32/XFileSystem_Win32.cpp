
#include "Oak/Platform/FileSystem/Win32/XFileSystem_Win32.hpp"


namespace Oak {


FileSystem::FileSystem()
    : m_fileHandle(NULL)
{}

FileSystem::~FileSystem()
{
    Close();
}

Bool FileSystem::Open(const Char* filename)
{
    m_fileHandle = CreateFileA(
        filename,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (m_fileHandle == INVALID_HANDLE_VALUE)
    {
        m_fileHandle = NULL;
        return false;
    }

    return true;
}

SizeT FileSystem::Read(Void* pBlock, SizeT size)
{
    DWORD readSize = 0;

    if (m_fileHandle)
    {
        ReadFile(
            m_fileHandle,
            reinterpret_cast<LPVOID>(pBlock),
            static_cast<DWORD>(size),
            &readSize,
            NULL
        );
    }

    return static_cast<SizeT>(readSize);
}

SizeT FileSystem::Write(const Void* pBlock, SizeT size)
{
    DWORD writtenSize = 0;

    if (m_fileHandle)
    {
        WriteFile(
            m_fileHandle,
            reinterpret_cast<LPCVOID>(pBlock),
            static_cast<DWORD>(size),
            &writtenSize,
            NULL
        );
    }

    return static_cast<SizeT>(writtenSize);
}

Void FileSystem::Close()
{
    if (m_fileHandle)
    {
        CloseHandle(m_fileHandle);
        m_fileHandle = NULL;
    }
}


} // namespace Oak

