
#include "Oak/Platform/Thread/Win32/XSemapore_Win32.hpp"

namespace Oak
{

Semaphore::Semaphore(const Char* name, Int32 initialCount, Int32 maxCount)
  : ISemaphore(name, initialCount, maxCount)
{
    strcpy_s(m_name, name);
    m_handle = CreateSemaphoreA(NULL, static_cast<LONG>(initialCount),
                                static_cast<LONG>(maxCount), m_name);
}

Semaphore::~Semaphore()
{
    CloseHandle(m_handle);
}

Void Semaphore::Lock()
{
    m_handle = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, m_name);
    WaitForSingleObject(m_handle, INFINITE);
}

Void Semaphore::Unlock()
{
    ReleaseSemaphore(m_handle, 1, NULL);
}

} // namespace Oak
