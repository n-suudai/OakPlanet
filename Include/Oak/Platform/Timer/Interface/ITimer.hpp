
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"


namespace Oak {
namespace Detail {


class IHighResolutionTimer
{
protected:
    IHighResolutionTimer() = default;

    virtual ~IHighResolutionTimer() = default;

public:
    virtual Double NowSeconds() const = 0;

    virtual Bool IsEnableHighResolutionTimer() const = 0;
};


} // namespace Detail


class StandardTimer final
{
private:
    StandardTimer() = delete;

    ~StandardTimer() = delete;

public:
    static Double NowSeconds();
};


} // namespace Oak

