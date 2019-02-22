
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{

class NedAllocatePolicyImpl
{
public:
    static Void* AllocateBytes(SizeT bytes);

    static Void* AllocateBytesAligned(SizeT bytes, SizeT alignment);

    static Void DeallocateBytes(Void* pBlock);

    static Void DeallocateBytesAligned(Void* pBlock, SizeT alignment);
};

class NedAllocatePolicy
{
public:
    static inline Void* AllocateBytes(SizeT bytes)
    {
        return NedAllocatePolicyImpl::AllocateBytes(bytes);
    }

    static inline Void* AllocateBytesAligned(SizeT bytes, SizeT alignment)
    {
        return NedAllocatePolicyImpl::AllocateBytesAligned(bytes, alignment);
    }

    static inline Void DeallocateBytes(Void* pBlock)
    {
        NedAllocatePolicyImpl::DeallocateBytes(pBlock);
    }

    static inline Void DeallocateBytesAligned(Void* pBlock, SizeT alignment)
    {
        NedAllocatePolicyImpl::DeallocateBytesAligned(pBlock, alignment);
    }
};

} // namespace Oak

#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED
