
#pragma once

#include "Oak/Platform/OS/Win32.hpp"
#include "Oak/Platform/AtomicDataTypes.hpp"


namespace Oak {


class Semaphore final
{
public:
    Semaphore(const Char* name, Int32 initialCount, Int32 maxCount);

    ~Semaphore();

    Void Lock();

    Void Unlock();

private:
    Char m_name[128];
    HANDLE m_handle;
};


} // namespace Oak

