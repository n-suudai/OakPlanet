
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Platform/STL/Container.hpp"

namespace Oak
{

template <typename Policy>
class STLAllocator
{
public:
    static inline Void* AllocateBytesAligned(SizeT bytes, SizeT alignment,
                                             const Char*, Int32, const Char*)
    {
        return Policy::AllocateBytesAligned(bytes, alignment);
    }

    static inline Void DeallocateBytesAligned(Void* pBlock, SizeT alignment)
    {
        Policy::DeallocateBytesAligned(pBlock, alignment);
    }
};

} // namespace Oak
