
#pragma once

#include "Oak/Platform/OS/Win32.hpp"
#include "Oak/Platform/Thread/Interface/ICriticalSection.hpp"


namespace Oak {


class CriticalSection final
    : public Detail::ICriticalSection
{
public:
    CriticalSection();

    ~CriticalSection();

    Void Lock() override;

    Void Unlock() override;

private:
    CRITICAL_SECTION m_criticalSection;
};


} // namespace Oak

