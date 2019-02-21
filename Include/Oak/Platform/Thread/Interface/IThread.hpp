
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{

typedef UInt32 (*ThreadEntry)(Void*, SizeT);

namespace Detail
{

class IThread
{
protected:
    IThread() = delete;

    IThread(const Char* /*name*/, ThreadEntry /*threadEntry*/)
    {
    }

    virtual ~IThread() = default;

public:
    virtual Void Start(Void* pArgumentBlock, SizeT argumentSize) = 0;

    virtual Int32 Wait() = 0;

    static Void Sleep(UInt32 milliSeconds);
};

} // namespace Detail
} // namespace Oak
