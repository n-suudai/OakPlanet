#include "Oak/Core/Memory/AlignedAllocator.hpp"
#include "Oak/Core/Memory/MemoryConfig.hpp"
#include "Oak/Core/Assert.hpp"
#include "Oak/Core/Bitwise.hpp"
#include <malloc.h>

namespace Oak
{

Void* AlignedMemory::Allocate(SizeT bytes, SizeT alignment)
{
    OAK_ASSERT(0 < alignment && alignment <= 128 &&
               Bitwise::IsPowerOf2(alignment));

    return _mm_malloc(bytes, alignment);
}

Void* AlignedMemory::Allocate(SizeT bytes)
{
    return Allocate(bytes, OAK_SIMD_ALIGNMENT);
}

Void AlignedMemory::Deallocate(Void* pBlock)
{
    if (!pBlock)
    {
        return;
    }

    _mm_free(pBlock);
}

} // namespace Oak
