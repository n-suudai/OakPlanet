
#pragma once


#include "Oak/Core/Memory/MemoryConfig.hpp"


#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NEDPOOLING


#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Platform/PlatformDetection.hpp"


namespace Oak {


class NedPoolingAllocatePolicyImpl
{
public:
    static DECL_MALLOC Void* AllocateBytes(SizeT bytes);

    static Void DeallocateBytes(Void* pBlock);

    static DECL_MALLOC Void* AllocateBytesAligned(SizeT alignment, SizeT bytes);

    static Void DeallocateBytesAligned(SizeT alignment, Void* pBlock);
};


class NedPoolingAllocatePolicy
{
public:
    static inline Void* AllocateBytes(SizeT bytes)
    {
        return NedPoolingAllocatePolicyImpl::AllocateBytes(bytes);
    }

    static inline Void DeallocateBytes(Void* pBlock)
    {
        NedPoolingAllocatePolicyImpl::DeallocateBytes(pBlock);
    }
};


template<SizeT Alignment = 0>
class NedPoolingAlignedAllocatePolicy
{
    // compile-time check alignment is available.
    typedef int IsValidAlignment[Alignment <= 128 && ((Alignment & (Alignment - 1)) == 0) ? +1 : -1];
public:
    static inline Void* AllocateBytes(SizeT bytes)
    {
        return NedPoolingAllocatePolicyImpl::AllocateBytesAligned(
            Alignment,
            bytes
        );
    }

    static inline Void DeallocateBytes(Void* pBlock)
    {
        NedPoolingAllocatePolicyImpl::DeallocateBytesAligned(
            Alignment,
            pBlock
        );
    }
};


} // namespace Oak


#endif  // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NEDPOOLING

