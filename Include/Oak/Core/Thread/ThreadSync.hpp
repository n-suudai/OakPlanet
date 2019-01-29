
#pragma once

#include "Oak/Core/Types.hpp"
#include  "Oak/Core/CopyMoveAssign.hpp"



namespace Oak {
namespace Core {
namespace Native {


class IMutex
{
    virtual ~IMutex() = default;

    virtual Void Lock() = 0;

    virtual Void Unlock() = 0;
};


class ICriticalSection
{
    virtual ~ICriticalSection() = default;

    virtual Void Lock() = 0;

    virtual Void Unlock() = 0;
};


class ISemaphore
{
    virtual ~ISemaphore() = default;

    virtual Bool WaitSignal() = 0;

    virtual Void OpenSignal() = 0;

    virtual Void ReleaseSignal() = 0;
};


class IEvent
{
    virtual ~IEvent() = default;

    virtual Bool WaitSignal() = 0;

    virtual Void SetSignal() = 0;

    virtual Void ResetSignal() = 0;
};


} // namespace Native



class Mutex
{
private:
    OAK_DISALLOW_COPY_MOVE_ASSIGN(Mutex);
public:
    Mutex();

    ~Mutex();

    Void Lock();

    Void Unlock();

private:
    Native::IMutex* m_pNativeMutex;
};


class CriticalSection
{
private:
    OAK_DISALLOW_COPY_MOVE_ASSIGN(CriticalSection);
public:
    CriticalSection();

    ~CriticalSection();

    Void Lock();

    Void Unlock();

private:
    Native::ICriticalSection* m_pNativeCriticalSection;
};


class Semaphore
{
private:
    OAK_DISALLOW_COPY_MOVE_ASSIGN(Semaphore);
public:
    Semaphore();

    ~Semaphore();

    Bool WaitSignal();

    Void OpenSignal();

    Void ReleaseSignal();

private:
    Native::ISemaphore* m_pNativeSemaphore;
};



class Event
{
private:
    OAK_DISALLOW_COPY_MOVE_ASSIGN(Event);
public:
    Event();

    ~Event();

    Bool WaitSignal();

    Void SetSignal();

    Void ResetSignal();

private:
    Native::IEvent* m_pNativeEvent;
};


} // namespace Core
} // namespace Oak

