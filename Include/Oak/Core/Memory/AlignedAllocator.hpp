
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"
#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Core/Assert.hpp"
#include "Oak/Core/Bitwise.hpp"

namespace Oak
{

/**
*
* |___2___|3|_________5__________|__6__|
* ^         ^
* 1         4
*
* 1 -> Pointer to start of the block allocated by new.
* 2 -> Gap used to get 4 aligned on given alignment
* 3 -> Byte offset between 1 and 4
* 4 -> Pointer to the start of data block.
* 5 -> Data block.
* 6 -> Wasted memory at rear of data block.
*/

class AlignedMemory
{
public:
    static inline Void* Allocate(SizeT bytes, SizeT alignment)
    {
        OAK_ASSERT(0 < alignment && alignment <= 128 &&
                   Bitwise::IsPowerOf2(alignment));

        UInt8* p = new UInt8[bytes + alignment];
        PtrDiff offset =
          alignment - (reinterpret_cast<PtrDiff>(p) & (alignment - 1));

        UInt8* pBlock = p + offset;
        pBlock[-1] = static_cast<UInt8>(offset);

        return pBlock;
    }

    static inline Void* Allocate(SizeT bytes)
    {
        return Allocate(bytes, OAK_SIMD_ALIGNMENT);
    }

    static inline Void Deallocate(Void* pBlock)
    {
        if (pBlock)
        {
            UInt8* p = reinterpret_cast<UInt8*>(pBlock);
            p = p - p[-1];
            delete[] p;
        }
    }
};

} // namespace Oak
