
#pragma once

#include "Oak/Platform/Timer/Interface/ITimer.hpp"


namespace Oak {


class HighResolutionTimer final
    : public Detail::IHighResolutionTimer
{
public:
    HighResolutionTimer();

    ~HighResolutionTimer();

    Double NowSeconds() const override;

    Bool IsEnableHighResolutionTimer() const override;

private:
    Bool    m_supportHighResolutionTimer;
    Double  m_secondsPerTick;
};


} // namespace Oak

