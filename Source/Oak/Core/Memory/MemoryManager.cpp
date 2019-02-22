
#include "Oak/Core/Memory/MemoryManager.hpp"
//#include "Oak/Core/Memory/AllocateConfig.hpp"

#if OAK_USE_HEAP_TRACKING

static Oak::Heap* s_pDefaultHeap = nullptr;

Oak::Void* operator new(Oak::SizeT bytes, const Oak::Char* file,
                        const Oak::Int32 line, const Oak::Char* function)
{
    if (s_pDefaultHeap == nullptr)
    {
        s_pDefaultHeap = Oak::HeapFactory::Get().GetDefaultHeap();
    }

    return s_pDefaultHeap->AllocateBytes(bytes, file, line, function);
}

Oak::Void* operator new [](Oak::SizeT bytes, const Oak::Char* file,
                           const Oak::Int32 line, const Oak::Char* function)
{
    if (s_pDefaultHeap == nullptr)
    {
        s_pDefaultHeap = Oak::HeapFactory::Get().GetDefaultHeap();
    }

    return s_pDefaultHeap->AllocateBytes(bytes, file, line, function);
} Oak::Void*
operator new(Oak::SizeT bytes)
{
    if (s_pDefaultHeap == nullptr)
    {
        s_pDefaultHeap = Oak::HeapFactory::Get().GetDefaultHeap();
    }

    return s_pDefaultHeap->AllocateBytes(bytes, __FILE__, __LINE__,
                                         __FUNCTION__);
}

Oak::Void* operator new [](Oak::SizeT bytes)
{
    if (s_pDefaultHeap == nullptr)
    {
        s_pDefaultHeap = Oak::HeapFactory::Get().GetDefaultHeap();
    }

    return s_pDefaultHeap->AllocateBytes(bytes, __FILE__, __LINE__,
                                         __FUNCTION__);
} Oak::Void
operator delete(Oak::Void* pBlock, const Oak::Char*, const Oak::Int32,
                const Oak::Char*)
{
    s_pDefaultHeap->DeallocateBytes(pBlock);
}

Oak::Void operator delete [](Oak::Void* pBlock, const Oak::Char*,
                             const Oak::Int32, const Oak::Char*)
{ s_pDefaultHeap->DeallocateBytes(pBlock); } Oak::Void
operator delete(Oak::Void* pBlock)
{
    s_pDefaultHeap->DeallocateBytes(pBlock);
}

Oak::Void operator delete [](Oak::Void* pBlock)
{ s_pDefaultHeap->DeallocateBytes(pBlock); }

#else // !OAK_USE_HEAP_TRACKING

Oak::Void* operator new(Oak::SizeT bytes, const Oak::Char*, const Oak::Int32,
                        const Oak::Char*)
{
    return Oak::AllocatePolicy::AllocateBytes(bytes);
}

Oak::Void* operator new [](Oak::SizeT bytes, const Oak::Char*, const Oak::Int32,
                           const Oak::Char*)
{ return Oak::AllocatePolicy::AllocateBytes(bytes); } Oak::Void*
operator new(Oak::SizeT bytes)
{
    return Oak::AllocatePolicy::AllocateBytes(bytes);
}

Oak::Void* operator new [](Oak::SizeT bytes)
{ return Oak::AllocatePolicy::AllocateBytes(bytes); } Oak::Void
operator delete(Oak::Void* pBlock, const Oak::Char*, const Oak::Int32,
                const Oak::Char*)
{
    Oak::AllocatePolicy::DeallocateBytes(pBlock);
}

Oak::Void operator delete [](Oak::Void* pBlock, const Oak::Char*,
                             const Oak::Int32, const Oak::Char*)
{ Oak::AllocatePolicy::DeallocateBytes(pBlock); } Oak::Void
operator delete(Oak::Void* pBlock)
{
    Oak::AllocatePolicy::DeallocateBytes(pBlock);
}

Oak::Void operator delete [](Oak::Void* pBlock)
{ Oak::AllocatePolicy::DeallocateBytes(pBlock); }

#endif // OAK_USE_HEAP_TRACKING