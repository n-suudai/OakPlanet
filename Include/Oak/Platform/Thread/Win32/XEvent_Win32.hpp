
#pragma once

#include "Oak/Platform/OS/Win32.hpp"
#include "Oak/Platform/AtomicDataTypes.hpp"


namespace Oak {


class Event final
{
public:
    Event(const Char* name, Bool initialState = false);

    ~Event();

    Void WaitSignal();

    Void Set();

    Void Reset();

private:
    Char m_name[128];
    HANDLE m_handle;
};


} // namespace Oak

