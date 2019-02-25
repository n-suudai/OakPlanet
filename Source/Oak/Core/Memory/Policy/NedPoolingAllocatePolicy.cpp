
#include "Oak/Core/Memory/Policy/NedPoolingAllocatePolicy.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NEDPOOLING

#include "Oak/Core/Assert.hpp"
#include "Oak/Core/Bitwise.hpp"
#include "Oak/ThirdParty/nedmalloc.hpp"
#include <algorithm>

namespace Oak
{
namespace NedPoolingAllocateInternal
{

const SizeT s_poolCount = 14; // Needs to be greater than 4
Void* s_poolFootprint = reinterpret_cast<Void*>(0xBB1AA45ABB1AA45A);
nedalloc::nedpool* s_pools[s_poolCount + 1] = {0};
nedalloc::nedpool* s_poolsAligned[s_poolCount + 1] = {0};

#if OAK_USE_HEAP_TRACKING

nedalloc::nedpool* s_poolForTracking = 0;

#endif // OAK_USE_HEAP_TRACKING

SizeT poolIDFromSize(SizeT a_reqSize)
{
    // Requests size 16 or smaller are allocated at a 4 byte granularity.
    // Requests size 17 or larger are allocated at a 16 byte granularity.
    // With a s_poolCount of 14, requests size 177 or larger go in the default
    // pool.

    // spreadsheet style =IF(B35<=16; FLOOR((B35-1)/4;1); MIN(FLOOR((B35-1)/16;
    // 1)
    // + 3; 14))

    SizeT poolID = 0;

    if (a_reqSize > 0)
    {
        if (a_reqSize <= 16)
        {
            poolID = (a_reqSize - 1) >> 2;
        }
        else
        {
            poolID = std::min<SizeT>(((a_reqSize - 1) >> 4) + 3, s_poolCount);
        }
    }

    return poolID;
}

DECL_MALLOC Void* internalAlloc(SizeT a_reqSize)
{
    SizeT poolID = poolIDFromSize(a_reqSize);
    nedalloc::nedpool* pool(0); // A pool pointer of 0 means the default pool.

    if (poolID < s_poolCount)
    {
        if (s_pools[poolID] == 0)
        {
            // Init pool if first use

            s_pools[poolID] = nedalloc::nedcreatepool(0, 8);
            nedalloc::nedpsetvalue(
              s_pools[poolID],
              s_poolFootprint); // All pools are stamped with a footprint
        }

        pool = s_pools[poolID];
    }

    return nedalloc::nedpmalloc(pool, a_reqSize);
}

DECL_MALLOC Void* internalAllocAligned(SizeT a_reqSize, SizeT a_align)
{
    SizeT poolID = poolIDFromSize(a_reqSize);
    nedalloc::nedpool* pool(0); // A pool pointer of 0 means the default pool.

    if (poolID < s_poolCount)
    {
        if (s_poolsAligned[poolID] == 0)
        {
            // Init pool if first use

            s_poolsAligned[poolID] = nedalloc::nedcreatepool(0, 8);
            nedalloc::nedpsetvalue(
              s_poolsAligned[poolID],
              s_poolFootprint); // All pools are stamped with a footprint
        }

        pool = s_poolsAligned[poolID];
    }

    return nedalloc::nedpmemalign(pool, a_align, a_reqSize);
}

Void internalFree(Void* a_mem)
{
    if (a_mem)
    {
        nedalloc::nedpool* pool(0);

        // nedalloc lets us get the pool pointer from the memory pointer
        Void* footprint = nedalloc::nedgetvalue(&pool, a_mem);

        // Check footprint
        if (footprint == s_poolFootprint)
        {
            // If we allocated the pool, deallocate from this pool...
            nedalloc::nedpfree(pool, a_mem);
        }
        else
        {
            // ...otherwise let nedalloc handle it.
            nedalloc::nedfree(a_mem);
        }
    }
}

#if OAK_USE_HEAP_TRACKING

DECL_MALLOC Void* internalAllocForTracking(SizeT a_reqSize)
{
    if (s_poolForTracking == 0)
    {
        // Init pool if first use

        s_poolForTracking = nedalloc::nedcreatepool(0, 8);
        nedalloc::nedpsetvalue(
          s_poolForTracking,
          s_poolFootprint); // All pools are stamped with a footprint
    }

    return nedalloc::nedpmalloc(s_poolForTracking, a_reqSize);
}

DECL_MALLOC Void* internalAllocAlignedForTracking(SizeT a_reqSize,
                                                  SizeT a_align)
{
    if (s_poolForTracking == 0)
    {
        // Init pool if first use

        s_poolForTracking = nedalloc::nedcreatepool(0, 8);
        nedalloc::nedpsetvalue(
          s_poolForTracking,
          s_poolFootprint); // All pools are stamped with a footprint
    }

    return nedalloc::nedpmemalign(s_poolForTracking, a_align, a_reqSize);
}

Void internalFreeForTracking(Void* a_mem)
{
    internalFree(a_mem);
}

#endif // OAK_USE_HEAP_TRACKING

} // namespace NedPoolingAllocateInternal

DECL_MALLOC Void* NedPoolingAllocatePolicy::AllocateBytes(SizeT bytes)
{
    return NedPoolingAllocateInternal::internalAlloc(bytes);
}

Void NedPoolingAllocatePolicy::DeallocateBytes(Void* pBlock)
{
    // deal with null
    if (!pBlock)
    {
        return;
    }

    NedPoolingAllocateInternal::internalFree(pBlock);
}

DECL_MALLOC Void*
NedPoolingAllocatePolicy::AllocateBytesAligned(SizeT bytes, SizeT alignment)
{
    OAK_ASSERT(0 < alignment && alignment <= 128 &&
               Bitwise::IsPowerOf2(alignment));

    // default to platform SIMD alignment if none specified
    return NedPoolingAllocateInternal::internalAllocAligned(
      bytes, alignment ? alignment : OAK_SIMD_ALIGNMENT);
}

Void NedPoolingAllocatePolicy::DeallocateBytesAligned(Void* pBlock,
                                                      SizeT alignment)
{
    alignment;

    // deal with null
    if (!pBlock)
    {
        return;
    }

    NedPoolingAllocateInternal::internalFree(pBlock);
}

#if OAK_USE_HEAP_TRACKING

DECL_MALLOC Void*
NedPoolingAllocatePolicy::AllocateBytesForTracking(SizeT bytes)
{
    return NedPoolingAllocateInternal::internalAllocForTracking(bytes);
}

DECL_MALLOC Void*
NedPoolingAllocatePolicy::AllocateBytesAlignedForTracking(SizeT bytes,
                                                          SizeT alignment)
{
    return NedPoolingAllocateInternal::internalAllocAlignedForTracking(
      bytes, alignment);
}

Void NedPoolingAllocatePolicy::DeallocateBytesForTracking(Void* pBlock)
{
    // deal with null
    if (!pBlock)
    {
        return;
    }

    NedPoolingAllocateInternal::internalFreeForTracking(pBlock);
}

Void
NedPoolingAllocatePolicy::DeallocateBytesAlignedForTracking(Void* pBlock,
                                                            SizeT alignment)
{
    alignment;

    // deal with null
    if (!pBlock)
    {
        return;
    }

    NedPoolingAllocateInternal::internalFreeForTracking(pBlock);
}

#endif // OAK_USE_HEAP_TRACKING

} // namespace Oak

#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NEDPOOLING
