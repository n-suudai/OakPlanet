
#pragma once

#include "Oak/Platform/OS/Win32.hpp"
#include "Oak/Platform/AtomicDataTypes.hpp"


namespace Oak {


class CriticalSection final
{
public:
    CriticalSection();

    ~CriticalSection();

    Void Lock();

    Void Unlock();

private:
    CRITICAL_SECTION m_criticalSection;
};


} // namespace Oak

