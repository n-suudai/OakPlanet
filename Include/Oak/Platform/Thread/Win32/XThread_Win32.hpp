
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Platform/OS/Win32.hpp"


namespace Oak {


typedef UInt32(*ThreadEntry)(Void*, SizeT);


class Thread final
{
public:
    Thread(const Char* name, ThreadEntry threadEntry);

    ~Thread();

    Void Start(Void* pArgumentBlock, SizeT argumentSize);

    Int32 Wait();

    static Void Sleep(UInt32 milliSeconds);

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

