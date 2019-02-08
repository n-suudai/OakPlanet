
#pragma once

namespace Oak {


template<typename Protection>
class LockGuard final
{
public:
    explicit LockGuard(Protection& protection)
        : m_protection(protection)
    {
        m_protection.Lock();
    }

    ~LockGuard()
    {
        m_protection.Unlock();
    }

private:
    // not copyable
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;

    // not movable
    LockGuard(LockGuard&&) = delete;
    LockGuard& operator=(LockGuard&&) = delete;

private:
    Protection& m_protection;
};


} // namespace Oak

