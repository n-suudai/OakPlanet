
#pragma once

#include "Oak/Platform/OS/Win32.hpp"
#include "Oak/Platform/Thread/Interface/IEvent.hpp"

namespace Oak
{

class Event final : public Detail::IEvent
{
public:
    Event(const Char* name, Bool initialState = false);

    ~Event();

    Void WaitSignal() override;

    Void SetSignal() override;

    Void ResetSignal() override;

private:
    Char m_name[128];
    HANDLE m_handle;
};

} // namespace Oak
