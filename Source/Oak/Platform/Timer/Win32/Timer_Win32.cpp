
#include "Oak/Platform/Timer/Win32/Timer_Win32.hpp"
#include "Oak/Platform/OS/Win32.hpp"


namespace Oak {


HighResolutionTimer::HighResolutionTimer()
    : m_secondsPerTick(0)
    , m_supportHighResolutionTimer(false)
{
    LARGE_INTEGER frequency;
    if (QueryPerformanceFrequency(&frequency))
    {
        m_supportHighResolutionTimer = frequency.QuadPart != 0;

        if (m_supportHighResolutionTimer)
        {
            m_secondsPerTick = 1.0 / frequency.QuadPart;
        }
    }
}

HighResolutionTimer::~HighResolutionTimer()
{

}

Double HighResolutionTimer::NowSeconds() const
{
    if (m_supportHighResolutionTimer)
    {
        LARGE_INTEGER performanceCount;
        if (QueryPerformanceCounter(&performanceCount))
        {
            return performanceCount.QuadPart * m_secondsPerTick;
        }
    }

    return StandardTimer::NowSeconds();
}

Bool HighResolutionTimer::IsEnableHighResolutionTimer() const
{
    return m_supportHighResolutionTimer;
}




Double StandardTimer::NowSeconds()
{
    return GetTickCount64() * 0.001;
}


} // namespace Oak

