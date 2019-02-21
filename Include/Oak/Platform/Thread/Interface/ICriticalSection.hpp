
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{
namespace Detail
{

class ICriticalSection
{
protected:
    ICriticalSection() = default;

    virtual ~ICriticalSection() = default;

public:
    virtual Void Lock() = 0;

    virtual Void Unlock() = 0;
};

} // namespace Detail
} // namespace Oak
