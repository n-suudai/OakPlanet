
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"



#ifdef new

#undef new

#endif


#ifdef delete

#undef delete

#endif


namespace Oak {


template<typename AllocatePolicy>
struct AllocatedObject
{
    explicit AllocatedObject() = default;

    ~AllocatedObject() = default;

    Void* operator new(SizeT bytes, const Char* file, Int32 line, const Char* func)
    {
        return AllocatePolicy::AllocateBytes(bytes, file, line, func);
    }

    Void* operator new(SizeT bytes)
    {
        return AllocatePolicy::AllocateBytes(bytes);
    }

    Void* operator new(SizeT bytes, Void* pBlock)
    {
        static_cast<Void>(bytes);
        return pBlock;
    }

    Void* operator new[](SizeT bytes, const Char* file, Int32 line, const Char* func)
    {
        return AllocatePolicy::AllocateBytes(bytes, file, line, func);
    }

    Void* operator new[](SizeT bytes)
    {
        return AllocatePolicy::AllocateBytes(bytes);
    }

    Void operator delete(Void* pBlock, const Char*, Int32, const Char*)
    {
        AllocatePolicy::DeallocateBytes(pBlock);
    }

    Void operator delete(Void* pBlock)
    {
        AllocatePolicy::DeallocateBytes(pBlock);
    }

    Void operator delete(Void* pBlock, Void*)
    {
        AllocatePolicy::DeallocateBytes(pBlock);
    }

    Void operator delete[](Void* pBlock, const Char*, Int32, const Char*)
    {
        AllocatePolicy::DeallocateBytes(pBlock);
    }

    Void operator delete[](Void* pBlock)
    {
        AllocatePolicy::DeallocateBytes(pBlock);
    }
};


} // namespace Oak

