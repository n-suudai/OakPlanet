
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"
#include "Oak/Core/Memory/HeapFactory.hpp"
#include "Oak/Core/Memory/Heap.hpp"

Oak::Void* operator new(Oak::SizeT bytes, const Oak::Char* file,
                        const Oak::Int32 line, const Oak::Char* function);
Oak::Void* operator new[](Oak::SizeT bytes, const Oak::Char* file,
                          const Oak::Int32 line, const Oak::Char* function);
Oak::Void* operator new(Oak::SizeT bytes);
Oak::Void* operator new[](Oak::SizeT bytes);
Oak::Void operator delete(Oak::Void* pBlock, const Oak::Char* file,
                          const Oak::Int32 line, const Oak::Char* function);
Oak::Void operator delete[](Oak::Void* pBlock, const Oak::Char* file,
                            const Oak::Int32 line, const Oak::Char* function);
Oak::Void operator delete(Oak::Void* pBlock);
Oak::Void operator delete[](Oak::Void* pBlock);

#if OAK_USE_HEAP_TRACKING

#define OAK_DECLARE_HEAP                                                       \
    \
private:                                                                       \
    static Oak::Heap* s_pHeap;                                                 \
    \
public:                                                                        \
    static Oak::Void* operator new(Oak::SizeT bytes, const Oak::Char* file,    \
                                   const Oak::Int32 line,                      \
                                   const Oak::Char* function);                 \
    static Oak::Void* operator new[](Oak::SizeT bytes, const Oak::Char* file,  \
                                     const Oak::Int32 line,                    \
                                     const Oak::Char* function);               \
    static Oak::Void* operator new(Oak::SizeT bytes);                          \
    static Oak::Void* operator new[](Oak::SizeT bytes);                        \
    static Oak::Void operator delete(Oak::Void* pBlock, const Oak::Char* file, \
                                     const Oak::Int32 line,                    \
                                     const Oak::Char* function);               \
    static Oak::Void operator delete[](                                        \
      Oak::Void* pBlock, const Oak::Char* file, const Oak::Int32 line,         \
      const Oak::Char* function);                                              \
    static Oak::Void operator delete(Oak::Void* pBlock);                       \
    static Oak::Void operator delete[](Oak::Void* pBlock);

#define OAK_DEFINE_HEAP(className, heapName, policyName)                        \
    Oak::Heap* className::s_pHeap = nullptr;                                    \
    Oak::Void* className::operator new(                                         \
      Oak::SizeT bytes, const Oak::Char* file, const Oak::Int32 line,           \
      const Oak::Char* function)                                                \
    {                                                                           \
        if (s_pHeap == nullptr)                                                 \
        {                                                                       \
            s_pHeap =                                                           \
              Oak::HeapFactory::Get().CreateHeap<policyName>(heapName);         \
        }                                                                       \
        return s_pHeap->AllocateBytes(bytes, file, line, function);             \
    }                                                                           \
    Oak::Void* className::operator new [](                                      \
      Oak::SizeT bytes, const Oak::Char* file, const Oak::Int32 line,           \
      const Oak::Char* function)                                                \
    {                                                                           \
        if (s_pHeap == nullptr)                                                 \
        {                                                                       \
            s_pHeap =                                                           \
              Oak::HeapFactory::Get().CreateHeap<policyName>(heapName);         \
        }                                                                       \
        return s_pHeap->AllocateBytes(bytes, file, line, function);             \
    } Oak::Void* className::operator new(Oak::SizeT bytes)                      \
    {                                                                           \
        if (s_pHeap == nullptr)                                                 \
        {                                                                       \
            s_pHeap =                                                           \
              Oak::HeapFactory::Get().CreateHeap<policyName>(heapName);         \
        }                                                                       \
        return s_pHeap->AllocateBytes(bytes, __FILE__, __LINE__,                \
                                      __FUNCTION__);                            \
    }                                                                           \
    Oak::Void* className::operator new [](Oak::SizeT bytes)                     \
    {                                                                           \
        if (s_pHeap == nullptr)                                                 \
        {                                                                       \
            s_pHeap =                                                           \
              Oak::HeapFactory::Get().CreateHeap<policyName>(heapName);         \
        }                                                                       \
        return s_pHeap->AllocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__); \
    } Oak::Void className::operator delete(Oak::Void* pBlock,                   \
                                           const Oak::Char*, const Oak::Int32,  \
                                           const Oak::Char*)                    \
    {                                                                           \
        s_pHeap->DeallocateBytes(pBlock);                                       \
    }                                                                           \
    Oak::Void className::operator delete [](                                    \
      Oak::Void* pBlock, const Oak::Char*, const Oak::Int32, const Oak::Char*)  \
    {                                                                           \
        s_pHeap->DeallocateBytes(pBlock);                                       \
    } Oak::Void className::operator delete(Oak::Void* pBlock)                   \
    {                                                                           \
        s_pHeap->DeallocateBytes(pBlock);                                       \
    }                                                                           \
    Oak::Void className::operator delete [](Oak::Void* pBlock)                  \
    { s_pHeap->DeallocateBytes(pBlock); }

#define OAK_DEFINE_HEAP_ALIGN(className, heapName, policyName, alignment)      \
    Oak::Heap* className::s_pHeap = nullptr;                                   \
    Oak::Void* className::operator new(                                        \
      Oak::SizeT bytes, const Oak::Char* file, const Oak::Int32 line,          \
      const Oak::Char* function)                                               \
    {                                                                          \
        if (s_pHeap == nullptr)                                                \
        {                                                                      \
            s_pHeap =                                                          \
              Oak::HeapFactory::Get().CreateHeap<policyName>(heapName);        \
        }                                                                      \
        return s_pHeap->AllocateAlignedBytes(bytes, alignment, file, line,     \
                                             function);                        \
    }                                                                          \
    Oak::Void* className::operator new [](                                     \
      Oak::SizeT bytes, const Oak::Char* file, const Oak::Int32 line,          \
      const Oak::Char* function)                                               \
    {                                                                          \
        if (s_pHeap == nullptr)                                                \
        {                                                                      \
            s_pHeap =                                                          \
              Oak::HeapFactory::Get().CreateHeap<policyName>(heapName);        \
        }                                                                      \
        return s_pHeap->AllocateAlignedBytes(bytes, alignment, file, line,     \
                                             function);                        \
    } Oak::Void* className::operator new(Oak::SizeT bytes)                     \
    {                                                                          \
        if (s_pHeap == nullptr)                                                \
        {                                                                      \
            s_pHeap =                                                          \
              Oak::HeapFactory::Get().CreateHeap<policyName>(heapName);        \
        }                                                                      \
        return s_pHeap->AllocateAlignedBytes(bytes, alignment, __FILE__,       \
                                             __LINE__, __FUNCTION__);          \
    }                                                                          \
    Oak::Void* className::operator new [](Oak::SizeT bytes)                    \
    {                                                                          \
        if (s_pHeap == nullptr)                                                \
        {                                                                      \
            s_pHeap =                                                          \
              Oak::HeapFactory::Get().CreateHeap<policyName>(heapName);        \
        }                                                                      \
        return s_pHeap->AllocateAlignedBytes(bytes, alignment, __FILE__,       \
                                             __LINE__, __FUNCTION__);          \
    } Oak::Void className::operator delete(Oak::Void* pBlock,                  \
                                           const Oak::Char*, const Oak::Int32, \
                                           const Oak::Char*)                   \
    {                                                                          \
        s_pHeap->DeallocateAlignedBytes(pBlock, alignment);                    \
    }                                                                          \
    Oak::Void className::operator delete [](                                   \
      Oak::Void* pBlock, const Oak::Char*, const Oak::Int32, const Oak::Char*) \
    {                                                                          \
        s_pHeap->DeallocateAlignedBytes(pBlock, alignment);                    \
    } Oak::Void className::operator delete(Oak::Void* pBlock)                  \
    {                                                                          \
        s_pHeap->DeallocateAlignedBytes(pBlock, alignment);                    \
    }                                                                          \
    Oak::Void className::operator delete [](Oak::Void* pBlock)                 \
    { s_pHeap->DeallocateAlignedBytes(pBlock, alignment); }

#define OAK_DEFINE_HIERARCHAL_HEAP(className, heapName, parentName,             \
                                   policyName)                                  \
    Oak::Heap* className::s_pHeap = nullptr;                                    \
    Oak::Void* className::operator new(                                         \
      Oak::SizeT bytes, const Oak::Char* file, const Oak::Int32 line,           \
      const Oak::Char* function)                                                \
    {                                                                           \
        if (s_pHeap == nullptr)                                                 \
        {                                                                       \
            s_pHeap = Oak::HeapFactory::Get().CreateHeap<policyName>(           \
              heapName, parentName);                                            \
        }                                                                       \
        return s_pHeap->AllocateBytes(bytes, file, line, function);             \
    }                                                                           \
    Oak::Void* className::operator new [](                                      \
      Oak::SizeT bytes, const Oak::Char* file, const Oak::Int32 line,           \
      const Oak::Char* function)                                                \
    {                                                                           \
        if (s_pHeap == nullptr)                                                 \
        {                                                                       \
            s_pHeap = Oak::HeapFactory::Get().CreateHeap<policyName>(           \
              heapName, parentName);                                            \
        }                                                                       \
        return s_pHeap->AllocateBytes(bytes, file, line, function);             \
    } Oak::Void* className::operator new(Oak::SizeT bytes)                      \
    {                                                                           \
        if (s_pHeap == nullptr)                                                 \
        {                                                                       \
            s_pHeap =                                                           \
              Oak::HeapFactory::Get().CreateHeap<policyName>(heapName);         \
        }                                                                       \
        return s_pHeap->AllocateBytes(bytes, __FILE__, __LINE__,                \
                                      __FUNCTION__);                            \
    }                                                                           \
    Oak::Void* className::operator new [](Oak::SizeT bytes)                     \
    {                                                                           \
        if (s_pHeap == nullptr)                                                 \
        {                                                                       \
            s_pHeap =                                                           \
              Oak::HeapFactory::Get().CreateHeap<policyName>(heapName);         \
        }                                                                       \
        return s_pHeap->AllocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__); \
    } Oak::Void className::operator delete(Oak::Void* pBlock,                   \
                                           const Oak::Char*, const Oak::Int32,  \
                                           const Oak::Char*)                    \
    {                                                                           \
        s_pHeap->DeallocateBytes(pBlock);                                       \
    }                                                                           \
    Oak::Void className::operator delete [](                                    \
      Oak::Void* pBlock, const Oak::Char*, const Oak::Int32, const Oak::Char*)  \
    {                                                                           \
        s_pHeap->DeallocateBytes(pBlock);                                       \
    } Oak::Void className::operator delete(Oak::Void* pBlock)                   \
    {                                                                           \
        s_pHeap->DeallocateBytes(pBlock);                                       \
    }                                                                           \
    Oak::Void className::operator delete [](Oak::Void* pBlock)                  \
    { s_pHeap->DeallocateBytes(pBlock); }

#define OAK_DEFINE_HIERARCHAL_HEAP_ALIGN(className, heapName, parentName,      \
                                         policyName, alignment)                \
    Oak::Heap* className::s_pHeap = nullptr;                                   \
    Oak::Void* className::operator new(                                        \
      Oak::SizeT bytes, const Oak::Char* file, const Oak::Int32 line,          \
      const Oak::Char* function)                                               \
    {                                                                          \
        if (s_pHeap == nullptr)                                                \
        {                                                                      \
            s_pHeap = Oak::HeapFactory::Get().CreateHeap<policyName>(          \
              heapName, parentName);                                           \
        }                                                                      \
        return s_pHeap->AllocateAlignedBytes(bytes, alignment, file, line,     \
                                             function);                        \
    }                                                                          \
    Oak::Void* className::operator new [](                                     \
      Oak::SizeT bytes, const Oak::Char* file, const Oak::Int32 line,          \
      const Oak::Char* function)                                               \
    {                                                                          \
        if (s_pHeap == nullptr)                                                \
        {                                                                      \
            s_pHeap = Oak::HeapFactory::Get().CreateHeap<policyName>(          \
              heapName, parentName);                                           \
        }                                                                      \
        return s_pHeap->AllocateAlignedBytes(bytes, alignment, file, line,     \
                                             function);                        \
    } Oak::Void* className::operator new(Oak::SizeT bytes)                     \
    {                                                                          \
        if (s_pHeap == nullptr)                                                \
        {                                                                      \
            s_pHeap = Oak::HeapFactory::Get().CreateHeap<policyName>(          \
              heapName, parentName);                                           \
        }                                                                      \
        return s_pHeap->AllocateAlignedBytes(bytes, alignment, __FILE__,       \
                                             __LINE__, __FUNCTION__);          \
    }                                                                          \
    Oak::Void* className::operator new [](Oak::SizeT bytes)                    \
    {                                                                          \
        if (s_pHeap == nullptr)                                                \
        {                                                                      \
            s_pHeap = Oak::HeapFactory::Get().CreateHeap<policyName>(          \
              heapName, parentName);                                           \
        }                                                                      \
        return s_pHeap->AllocateAlignedBytes(bytes, alignment, __FILE__,       \
                                             __LINE__, __FUNCTION__);          \
    } Oak::Void className::operator delete(Oak::Void* pBlock,                  \
                                           const Oak::Char*, const Oak::Int32, \
                                           const Oak::Char*)                   \
    {                                                                          \
        s_pHeap->DeallocateAlignedBytes(pBlock, alignment);                    \
    }                                                                          \
    Oak::Void className::operator delete [](                                   \
      Oak::Void* pBlock, const Oak::Char*, const Oak::Int32, const Oak::Char*) \
    {                                                                          \
        s_pHeap->DeallocateAlignedBytes(pBlock, alignment);                    \
    } Oak::Void className::operator delete(Oak::Void* pBlock)                  \
    {                                                                          \
        s_pHeap->DeallocateAlignedBytes(pBlock, alignment);                    \
    }                                                                          \
    Oak::Void className::operator delete [](Oak::Void* pBlock)                 \
    { s_pHeap->DeallocateAlignedBytes(pBlock, alignment); }

#define OAK_NEW new (__FILE__, __LINE__, __FUNCTION__)
#define OAK_DELETE delete

#else // !OAK_USE_HEAP_TRACKING

#define OAK_DECLARE_HEAP                                                       \
    \
public:                                                                        \
    static Oak::Void* operator new(Oak::SizeT bytes, const Oak::Char* file,    \
                                   const Oak::Int32 line,                      \
                                   const Oak::Char* function);                 \
    static Oak::Void* operator new[](Oak::SizeT bytes, const Oak::Char* file,  \
                                     const Oak::Int32 line,                    \
                                     const Oak::Char* function);               \
    static Oak::Void* operator new(Oak::SizeT bytes);                          \
    static Oak::Void* operator new[](Oak::SizeT bytes);                        \
    static Oak::Void operator delete(Oak::Void* pBlock, const Oak::Char* file, \
                                     const Oak::Int32 line,                    \
                                     const Oak::Char* function);               \
    static Oak::Void operator delete[](                                        \
      Oak::Void* pBlock, const Oak::Char* file, const Oak::Int32 line,         \
      const Oak::Char* function);                                              \
    static Oak::Void operator delete(Oak::Void* pBlock);                       \
    static Oak::Void operator delete[](Oak::Void* pBlock);

#define OAK_DEFINE_HEAP(className, heapName, policyName)                       \
    Oak::Void* className::operator new(Oak::SizeT bytes, const Oak::Char*,     \
                                       const Oak::Int32, const Oak::Char*)     \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    }                                                                          \
    Oak::Void* className::operator new [](Oak::SizeT bytes, const Oak::Char*,  \
                                          const Oak::Int32, const Oak::Char*)  \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    } Oak::Void* className::operator new(Oak::SizeT bytes)                     \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    }                                                                          \
    Oak::Void* className::operator new [](Oak::SizeT bytes)                    \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    } Oak::Void className::operator delete(Oak::Void* pBlock,                  \
                                           const Oak::Char*, const Oak::Int32, \
                                           const Oak::Char*)                   \
    {                                                                          \
        policyName::DeallocateBytes(pBlock);                                   \
    }                                                                          \
    Oak::Void className::operator delete [](                                   \
      Oak::Void* pBlock, const Oak::Char*, const Oak::Int32, const Oak::Char*) \
    {                                                                          \
        policyName::DeallocateBytes(pBlock);                                   \
    } Oak::Void className::operator delete(Oak::Void* pBlock)                  \
    {                                                                          \
        policyName::DeallocateBytes(pBlock);                                   \
    }                                                                          \
    Oak::Void className::operator delete [](Oak::Void* pBlock)                 \
    { policyName::DeallocateBytes(pBlock); }

#define OAK_DEFINE_HEAP_ALIGN(className, heapName, policyName, alignment)      \
    Oak::Void* className::operator new(Oak::SizeT bytes, const Oak::Char*,     \
                                       const Oak::Int32, const Oak::Char*)     \
    {                                                                          \
        return policyName::AllocateBytesAligned(bytes, alignment);             \
    }                                                                          \
    Oak::Void* className::operator new [](Oak::SizeT bytes, const Oak::Char*,  \
                                          const Oak::Int32, const Oak::Char*)  \
    {                                                                          \
        return policyName::AllocateBytesAligned(bytes, alignment);             \
    } Oak::Void* className::operator new(Oak::SizeT bytes)                     \
    {                                                                          \
        return policyName::AllocateBytesAligned(bytes, alignment);             \
    }                                                                          \
    Oak::Void* className::operator new [](Oak::SizeT bytes)                    \
    {                                                                          \
        return policyName::AllocateBytesAligned(bytes, alignment);             \
    } Oak::Void className::operator delete(Oak::Void* pBlock,                  \
                                           const Oak::Char*, const Oak::Int32, \
                                           const Oak::Char*)                   \
    {                                                                          \
        policyName::DeallocateBytesAligned(pBlock, alignment);                 \
    }                                                                          \
    Oak::Void className::operator delete [](                                   \
      Oak::Void* pBlock, const Oak::Char*, const Oak::Int32, const Oak::Char*) \
    {                                                                          \
        policyName::DeallocateBytesAligned(pBlock, alignment);                 \
    } Oak::Void className::operator delete(Oak::Void* pBlock)                  \
    {                                                                          \
        policyName::DeallocateBytesAligned(pBlock, alignment);                 \
    }                                                                          \
    Oak::Void className::operator delete [](Oak::Void* pBlock)                 \
    { policyName::DeallocateBytesAligned(pBlock, alignment); }

#define OAK_DEFINE_HIERARCHAL_HEAP(className, heapName, parentName,            \
                                   policyName)                                 \
    Oak::Void* className::operator new(Oak::SizeT bytes, const Oak::Char*,     \
                                       const Oak::Int32, const Oak::Char*)     \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    }                                                                          \
    Oak::Void* className::operator new [](Oak::SizeT bytes, const Oak::Char*,  \
                                          const Oak::Int32, const Oak::Char*)  \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    } Oak::Void* className::operator new(Oak::SizeT bytes)                     \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    }                                                                          \
    Oak::Void* className::operator new [](Oak::SizeT bytes)                    \
    {                                                                          \
        return policyName::AllocateBytes(bytes);                               \
    } Oak::Void className::operator delete(Oak::Void* pBlock,                  \
                                           const Oak::Char*, const Oak::Int32, \
                                           const Oak::Char*)                   \
    {                                                                          \
        policyName::DeallocateBytes(pBlock);                                   \
    }                                                                          \
    Oak::Void className::operator delete [](                                   \
      Oak::Void* pBlock, const Oak::Char*, const Oak::Int32, const Oak::Char*) \
    {                                                                          \
        policyName::DeallocateBytes(pBlock);                                   \
    } Oak::Void className::operator delete(Oak::Void* pBlock)                  \
    {                                                                          \
        policyName::DeallocateBytes(pBlock);                                   \
    }                                                                          \
    Oak::Void className::operator delete [](Oak::Void* pBlock)                 \
    { policyName::DeallocateBytes(pBlock); }

#define OAK_DEFINE_HIERARCHAL_HEAP_ALIGN(className, heapName, parentName,      \
                                         policyName, alignment)                \
    Oak::Void* className::operator new(Oak::SizeT bytes, const Oak::Char*,     \
                                       const Oak::Int32, const Oak::Char*)     \
    {                                                                          \
        return policyName::AllocateBytesAligned(bytes, alignment);             \
    }                                                                          \
    Oak::Void* className::operator new [](Oak::SizeT bytes, const Oak::Char*,  \
                                          const Oak::Int32, const Oak::Char*)  \
    {                                                                          \
        return policyName::AllocateBytesAligned(bytes, alignment);             \
    } Oak::Void* className::operator new(Oak::SizeT bytes)                     \
    {                                                                          \
        return policyName::AllocateBytesAligned(bytes, alignment);             \
    }                                                                          \
    Oak::Void* className::operator new [](Oak::SizeT bytes)                    \
    {                                                                          \
        return policyName::AllocateBytesAligned(bytes, alignment);             \
    } Oak::Void className::operator delete(Oak::Void* pBlock,                  \
                                           const Oak::Char*, const Oak::Int32, \
                                           const Oak::Char*)                   \
    {                                                                          \
        policyName::DeallocateBytesAligned(pBlock, alignment);                 \
    }                                                                          \
    Oak::Void className::operator delete [](                                   \
      Oak::Void* pBlock, const Oak::Char*, const Oak::Int32, const Oak::Char*) \
    {                                                                          \
        policyName::DeallocateBytesAligned(pBlock, alignment);                 \
    } Oak::Void className::operator delete(Oak::Void* pBlock)                  \
    {                                                                          \
        policyName::DeallocateBytesAligned(pBlock, alignment);                 \
    }                                                                          \
    Oak::Void className::operator delete [](Oak::Void* pBlock)                 \
    { policyName::DeallocateBytesAligned(pBlock, alignment); }

#define OAK_NEW new
#define OAK_DELETE delete

#endif // OAK_USE_HEAP_TRACKING
