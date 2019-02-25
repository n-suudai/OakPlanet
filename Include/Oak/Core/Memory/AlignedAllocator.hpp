
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{

class AlignedMemory
{
public:
    static Void* Allocate(SizeT bytes, SizeT alignment);

    static Void* Allocate(SizeT bytes);

    static Void Deallocate(Void* pBlock);
};

} // namespace Oak
