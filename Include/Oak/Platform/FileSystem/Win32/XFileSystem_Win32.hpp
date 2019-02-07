
#pragma once

#include "Oak/Platform/FileSystem/Interface/IFileSystem.hpp"
#include "Oak/Platform/OS/Win32.hpp"


namespace Oak {


class FileSystem final
    : public Detail::IFileSystem
{
public:
    FileSystem();

    ~FileSystem();

    Bool Open(const Char* filename) override;

    SizeT Read(Void* pBlock, SizeT size) override;

    SizeT Write(const Void* pBlock, SizeT size) override;

    Void Close() override;

private:
    HANDLE m_fileHandle;

};


} // namespace Oak

