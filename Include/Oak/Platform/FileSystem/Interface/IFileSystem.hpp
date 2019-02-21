
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{

namespace Detail
{

class IFileSystem
{
protected:
    IFileSystem() = default;

    virtual ~IFileSystem() = default;

public:
    virtual Bool Open(const Char* filename) = 0;

    virtual SizeT Read(Void* pBlock, SizeT size) = 0;

    virtual SizeT Write(const Void* pBlock, SizeT size) = 0;

    virtual Void Close() = 0;
};

} // namespace Detail
} // namespace Oak
