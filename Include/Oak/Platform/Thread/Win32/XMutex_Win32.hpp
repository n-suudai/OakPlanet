
#pragma once


#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Platform/OS/Win32.hpp"



namespace Oak {


class Mutex final
{
public:
    Mutex(const Char* name);

    ~Mutex();

    Int32 Lock();

    Int32 Unlock();

private:
    Char m_name[128];
    HANDLE m_handle;
};



} // namespace Oak

