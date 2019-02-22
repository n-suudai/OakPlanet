
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NEDPOOLING

#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Platform/PlatformDetection.hpp"

namespace Oak
{

class NedPoolingAllocatePolicyImpl
{
public:
    static DECL_MALLOC Void* AllocateBytes(SizeT bytes);

    static DECL_MALLOC Void* AllocateBytesAligned(SizeT bytes, SizeT alignment);

    static Void DeallocateBytes(Void* pBlock);

    static Void DeallocateBytesAligned(Void* pBlock, SizeT alignment);
};

class NedPoolingAllocatePolicy
{
public:
    static inline DECL_MALLOC Void* AllocateBytes(SizeT bytes)
    {
        return NedPoolingAllocatePolicyImpl::AllocateBytes(bytes);
    }

    static inline DECL_MALLOC Void* AllocateBytesAligned(SizeT bytes,
                                                         SizeT alignment)
    {
        return NedPoolingAllocatePolicyImpl::AllocateBytesAligned(bytes,
                                                                  alignment);
    }

    static inline Void DeallocateBytes(Void* pBlock)
    {
        NedPoolingAllocatePolicyImpl::DeallocateBytes(pBlock);
    }

    static inline Void DeallocateBytesAligned(Void* pBlock, SizeT alignment)
    {
        NedPoolingAllocatePolicyImpl::DeallocateBytesAligned(pBlock, alignment);
    }
};

} // namespace Oak

#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NEDPOOLING
