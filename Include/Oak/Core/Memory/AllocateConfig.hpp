
#pragma once

#include "Oak/Core/Memory/AllocatedObject.hpp"
#include "Oak/Core/Memory/MemoryConfig.hpp"


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


#include "Oak/Core/Memory/Policy/NedAllocatePolicy.hpp"

namespace Oak {

template<MEMORY_CATEGORY Category>
class CategorisedAllocatePolicy : public NedAllocatePolicy {};

template<MEMORY_CATEGORY Category, SizeT Alignment = 0>
class CategorisedAlignAllocatePolicy : public NedAlignedAllocatePolicy<Alignment> {};

} // namespace Oak


#elif OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD


#include "Oak/Core/Memory/Policy/StandardAllocatePolicy.hpp"

namespace Oak {

template<MEMORY_CATEGORY Category>
class CategorisedAllocatePolicy : public StandardAllocatePolicy {};

template<MEMORY_CATEGORY Category, SizeT Alignment = 0>
class CategorisedAlignAllocatePolicy : public StandardAlignedAllocatePolicy<Alignment> {};

} // namespace Oak


#else

#error "does not defined allocator."

#endif


namespace  Oak {


// policy shortcuts
typedef CategorisedAllocatePolicy<MEMORY_CATEGORY_GENERAL> GeneralAllocatePolicy;


// aligned policy shortcuts
template<SizeT Alignment = 0>
using GeneralAlignedAllocatePolicy = CategorisedAlignAllocatePolicy<MEMORY_CATEGORY_GENERAL, Alignment>;


// base classes
typedef AllocatedObject<GeneralAllocatePolicy> GeneralAllocatedObject;


// aligned base classes
template<SizeT Alignment = 0>
using GeneralAlignAllocatedObject = AllocatedObject<GeneralAlignedAllocatePolicy<Alignment>>;


} // namespace Oak



#if OAK_BUILD_DEBUG

#define OAK_NEW new(__FILE__, __LINE__, __FUNCTION__)
#define OAK_DELETE delete

#else // !OAK_BUILD_DEBUG

#define OAK_NEW new
#define OAK_DELETE delete

#endif

