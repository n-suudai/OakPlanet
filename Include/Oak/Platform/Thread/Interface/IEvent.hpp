
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"


namespace Oak {
namespace Detail {


class IEvent
{
protected:
    IEvent() = delete;

    IEvent(const Char* /*name*/, Bool /*initialState = false*/) {}

    virtual ~IEvent() = default;

public:
    virtual Void WaitSignal() = 0;

    virtual Void SetSignal() = 0;

    virtual Void ResetSignal() = 0;
};


} // namespace Detail
} // namespace Oak

