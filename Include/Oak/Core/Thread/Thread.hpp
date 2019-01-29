
#pragma once

#include "Oak/Core/Types.hpp"
#include "Oak/Core/CopyMoveAssign.hpp"


namespace Oak {
namespace Core {
namespace Native {


class IThread
{
public:
    virtual ~IThread() = default;

    virtual Void Create(const Char* name, ThreadEntry threadEntry) = 0;

    virtual Int32 Start(SizeT argumentSize, const Void* pArgumentBlock) = 0;

    virtual Void WaitEnd() = 0;
};


} // namespace Native


typedef Void(*ThreadEntry)(SizeT argumentSize, const Void* pArgumentBlock);


class Thread
{
private:
    OAK_DISALLOW_COPY_MOVE_ASSIGN(Thread);
public:
    Thread();

    ~Thread();

    Void Create(const Char* name, ThreadEntry threadEntry);

    Int32 Start(SizeT argumentSize, const Void* pArgumentBlock);

    Void WaitEnd();

private:
    Thread* m_pNativeThread;
};


} // namespace Core
} // namespace Oak

