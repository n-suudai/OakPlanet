
#include "Oak/Platform/Thread/Win32/XCriticalSection_Win32.hpp"


namespace Oak {


CriticalSection::CriticalSection()
{
    InitializeCriticalSection(&m_criticalSection);
}

CriticalSection::~CriticalSection()
{
    DeleteCriticalSection(&m_criticalSection);
}

Void CriticalSection::Lock()
{
    EnterCriticalSection(&m_criticalSection);
}

Void CriticalSection::Unlock()
{
    LeaveCriticalSection(&m_criticalSection);
}


} // namespace Oak

