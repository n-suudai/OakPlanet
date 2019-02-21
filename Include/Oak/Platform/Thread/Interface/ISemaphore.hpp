
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{
namespace Detail
{

class ISemaphore
{
protected:
    ISemaphore() = delete;

    ISemaphore(const Char* /*name*/, Int32 /*initialCount*/, Int32 /*maxCount*/)
    {
    }

    virtual ~ISemaphore() = default;

public:
    virtual Void Lock() = 0;

    virtual Void Unlock() = 0;
};

} // namespace Detail
} // namespace Oak
