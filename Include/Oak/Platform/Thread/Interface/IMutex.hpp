
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{
namespace Detail
{

class IMutex
{
protected:
    IMutex() = delete;

    explicit IMutex(const Char* /*name*/)
    {
    }

    virtual ~IMutex() = default;

public:
    virtual Void Lock() = 0;

    virtual Void Unlock() = 0;
};

} // namespace Detail
} // namespace Oak
