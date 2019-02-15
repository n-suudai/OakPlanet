

#include "Oak/Core/Memory/Policy/NedAllocatePolicy.hpp"


#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED


#include "Oak/Core/Memory/MemoryTracker.hpp"
#include "Oak/ThirdParty/nedmalloc.hpp"
#include <algorithm>


namespace Oak {


Void* NedAllocatePolicyImpl::AllocateBytes(
    SizeT bytes,
    const Char* file,
    Int32 line,
    const Char* func
)
{
    Void* pBlock = nedalloc::nedmalloc(bytes);
#if OAK_MEMORY_TRACKER
    // this alloc policy doesn't do pools (yet, ned can do it)
    MemoryTracker::Get().RecordAllocation(pBlock, bytes, 0, file, line, func);
#endif
    return pBlock;
}

Void NedAllocatePolicyImpl::DeallocateBytes(Void* pBlock)
{
    // deal with null
    if (!pBlock) { return; }
#if OAK_MEMORY_TRACKER
    MemoryTracker::Get().RecordDeallocation(pBlock);
#endif
    nedalloc::nedfree(pBlock);
}

Void* NedAllocatePolicyImpl::AllocateBytesAligned(
    SizeT align,
    SizeT bytes,
    const Char* file,
    Int32 line,
    const Char* func
)
{
    // default to platform SIMD alignment if none specified
    Void* pBlock = nedalloc::nedmemalign(align ? align : OAK_SIMD_ALIGNMENT, bytes);
#if OAK_MEMORY_TRACKER
    // this alloc policy doesn't do pools (yet, ned can do it)
    MemoryTracker::Get().RecordAllocation(pBlock, bytes, 0, file, line, func);
#endif
    return pBlock;
}

Void NedAllocatePolicyImpl::DeallocateBytesAligned(SizeT alignment, Void* pBlock)
{
    alignment;

    // deal with null
    if (!pBlock) { return; }
#if OAK_MEMORY_TRACKER
    // this alloc policy doesn't do pools (yet, ned can do it)
    MemoryTracker::Get().RecordDeallocation(pBlock);
#endif
    nedalloc::nedfree(pBlock);
}


}  // namespace  Oak


#endif  // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED

