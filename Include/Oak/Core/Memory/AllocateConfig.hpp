
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NEDPOOLING

#include "Oak/Core/Memory/Policy/NedPoolingAllocatePolicy.hpp"

namespace Oak
{

typedef NedPoolingAllocatePolicy AllocatePolicy;

} // namespace Oak

#elif OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED

#include "Oak/Core/Memory/Policy/NedAllocatePolicy.hpp"

namespace Oak
{

typedef NedAllocatePolicy AllocatePolicy;

} // namespace Oak

#elif OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD

#include "Oak/Core/Memory/Policy/StandardAllocatePolicy.hpp"

namespace Oak
{

typedef StandardAllocatePolicy AllocatePolicy;

} // namespace Oak

#else

#error "does not defined allocator."

#endif
