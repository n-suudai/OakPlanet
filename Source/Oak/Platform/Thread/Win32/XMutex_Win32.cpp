
#include "Oak/Platform/Thread/Win32/XMutex_Win32.hpp"


namespace Oak {


Mutex::Mutex(const Char* name)
{
    strcpy_s(m_name, name);
    m_handle = CreateMutexA(NULL, FALSE, m_name);
}

Mutex::~Mutex()
{
    CloseHandle(m_handle);
}

Void Mutex::Lock()
{
    m_handle = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, m_name);
    WaitForSingleObject(m_handle, INFINITE);
}

Void Mutex::Unlock()
{
    ReleaseMutex(m_handle);
}



} // namespace Oak

