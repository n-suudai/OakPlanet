
#pragma once

#include "Oak/Platform/OS/Win32.hpp"
#include "Oak/Platform/Thread/Interface/ISemaphore.hpp"

namespace Oak
{

class Semaphore final : public Detail::ISemaphore
{
public:
    Semaphore(const Char* name, Int32 initialCount, Int32 maxCount);

    ~Semaphore();

    Void Lock() override;

    Void Unlock() override;

private:
    Char m_name[128];
    HANDLE m_handle;
};

} // namespace Oak
