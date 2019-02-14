
#pragma once

#include "Oak/Core/Memory/AllocatedObject.hpp"


#define OAK_MEMORY_ALLOCATOR_NEDPOOLING     1
#define OAK_MEMORY_ALLOCATOR_NED            2
#define OAK_MEMORY_ALLOCATOR_STD            3

#define OAK_MEMORY_ALLOCATOR OAK_MEMORY_ALLOCATOR_NEDPOOLING


namespace Oak {


enum MEMORY_CATEGORY : UInt8
{
    MEMORY_CATEGORY_GENERAL

    // do not use
    , MEMORY_CATEGORY_NUM
};

} // namespace Oak

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NEDPOOLING

#include "Oak/Core/Memory/Policy/NedPoolingAllocatePolicy.hpp"

namespace Oak {

template<MEMORY_CATEGORY Category>
class CategorisedAllocatePolicy : public NedPoolingAllocatePolicy {};

template<MEMORY_CATEGORY Category, SizeT Alignment = 0>
class CategorisedAlignAllocatePolicy : public NedPoolingAlignedAllocatePolicy<Alignment> {};

} // namespace Oak

#elif OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED

#elif OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD

#else

#error "does not defined allocator."

#endif


namespace  Oak {

// policy shortcuts
typedef CategorisedAllocatePolicy<MEMORY_CATEGORY_GENERAL> GeneralAllocatePolicy;


// aligned policy shortcuts
template<SizeT Alignment = 0>
using GeneralAlignedAllocatePolicy = CategorisedAlignAllocatePolicy<MEMORY_CATEGORY_GENERAL, Alignement>;


// base classes
typedef AllocatedObject<GeneralAllocatePolicy> GeneralAllocatedObject;


// aligned base classes
template<SizeT Alignment = 0>
using GeneralAlignAllocatedObject = AllocatedObject<CategorisedAlignAllocatePolicy<Alignment>>;



} // namespace Oak



#if OAK_BUILD_DEBUG

#define OAK_NEW new(__FILE__, __LINE__, __FUNCTION__)
#define OAK_DELETE delete

#else // !OAK_BUILD_DEBUG

#define OAK_NEW new
#define OAK_DELETE delete

#endif

