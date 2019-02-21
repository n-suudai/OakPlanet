
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED

#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Platform/PlatformDetection.hpp"

namespace Oak
{

class NedAllocatePolicyImpl
{
public:
    static Void* AllocateBytes(SizeT bytes);

    static Void DeallocateBytes(Void* pBlock);

    static Void* AllocateBytesAligned(SizeT alignment, SizeT bytes);

    static Void DeallocateBytesAligned(SizeT alignment, Void* pBlock);
};

class NedAllocatePolicy
{
public:
    static inline Void* AllocateBytes(SizeT bytes)
    {
        return NedAllocatePolicyImpl::AllocateBytes(bytes);
    }

    static inline Void DeallocateBytes(Void* pBlock)
    {
        NedAllocatePolicyImpl::DeallocateBytes(pBlock);
    }
};

template <SizeT Alignment = 0>
class NedAlignedAllocatePolicy
{
    // compile-time check alignment is available.
    typedef int IsValidAlignment
      [Alignment <= 128 && ((Alignment & (Alignment - 1)) == 0) ? +1 : -1];

public:
    static inline Void* AllocateBytes(SizeT bytes)
    {
        return NedAllocatePolicyImpl::AllocateBytesAligned(Alignment, bytes);
    }

    static inline Void DeallocateBytes(Void* pBlock)
    {
        NedAllocatePolicyImpl::DeallocateBytesAligned(Alignment, pBlock);
    }
};

} // namespace Oak

#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED
