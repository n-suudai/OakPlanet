#include "Oak/Platform/Thread/Win32/XThread_Win32.hpp"

namespace Oak
{

namespace Detail
{

Void IThread::Sleep(UInt32 milliSeconds)
{
    ::Sleep(static_cast<DWORD>(milliSeconds));
}

} // namespace Detail

Thread::Thread(const Char* name, ThreadEntry threadEntry)
  : IThread(name, threadEntry)
{
    strcpy_s(m_name, name);

    m_threadEntry = threadEntry;

    m_handle = CreateThread(NULL, 0, &Thread::ThreadStartRoutine,
                            reinterpret_cast<LPVOID>(this), CREATE_SUSPENDED,
                            &m_threadId);

    m_pArgumentBlock = nullptr;
    m_argumentSize = 0;
}

Thread::~Thread()
{
    CloseHandle(m_handle);
}

Void Thread::Start(Void* pArgumentBlock, SizeT argumentSize)
{
    ResumeThread(m_handle);
    m_pArgumentBlock = pArgumentBlock;
    m_argumentSize = argumentSize;
}

Int32 Thread::Wait()
{
    DWORD exitCode = STILL_ACTIVE;

    while (exitCode == STILL_ACTIVE)
    {
        GetExitCodeThread(m_handle, &exitCode);
    }

    return static_cast<Int32>(exitCode);
}

DWORD WINAPI Thread::ThreadStartRoutine(LPVOID lpArgument)
{
    Thread* pSelf = reinterpret_cast<Thread*>(lpArgument);

    DWORD exitCode = 0xFFFFFFFF;

    if (pSelf)
    {
        exitCode = static_cast<DWORD>(
          pSelf->m_threadEntry(pSelf->m_pArgumentBlock, pSelf->m_argumentSize));
    }

    return exitCode;
}

} // namespace Oak
