
#pragma once


#include "Oak/Platform/OS/Win32.hpp"
#include "Oak/Platform/Thread/Interface/IMutex.hpp"



namespace Oak {


class Mutex final
    : public Detail::IMutex
{
public:
    explicit Mutex(const Char* name);

    ~Mutex();

    Void Lock() override;

    Void Unlock() override;

private:
    Char m_name[128];
    HANDLE m_handle;
};



} // namespace Oak

