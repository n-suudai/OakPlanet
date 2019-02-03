
#include "Oak/Platform/Thread/Win32/XEvent_Win32.hpp"


namespace Oak {


Event::Event(const Char* name, Bool initialState)
{
    strcpy_s(m_name, name);

    m_handle = CreateEventA(
        NULL,
        TRUE,
        initialState ? TRUE : FALSE,
        m_name
    );

    m_handle = OpenEventA(EVENT_ALL_ACCESS, FALSE, m_name);
}

Event::~Event()
{
    CloseHandle(m_handle);
}

Void Event::WaitSignal()
{
    WaitForSingleObject(m_handle, INFINITE);
}

Void Event::Set()
{
    SetEvent(m_handle);
}

Void Event::Reset()
{
    ResetEvent(m_handle);
}


} // namespace Oak

