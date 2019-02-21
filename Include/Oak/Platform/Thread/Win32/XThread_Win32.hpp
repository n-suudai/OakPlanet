
#pragma once

#include "Oak/Platform/OS/Win32.hpp"
#include "Oak/Platform/Thread/Interface/IThread.hpp"

namespace Oak
{

class Thread final : public Detail::IThread
{
public:
    Thread(const Char* name, ThreadEntry threadEntry);

    ~Thread();

    Void Start(Void* pArgumentBlock, SizeT argumentSize) override;

    Int32 Wait() override;

private:
    static DWORD WINAPI ThreadStartRoutine(LPVOID lpArgument);

private:
    Char m_name[128];
    ThreadEntry m_threadEntry;
    HANDLE m_handle;
    DWORD m_threadId;
    Void* m_pArgumentBlock;
    SizeT m_argumentSize;
};

} // namespace Oak
