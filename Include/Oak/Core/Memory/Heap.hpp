
#pragma once

#include "Oak/Core/Types.hpp"
#include "Oak/Core/Thread/ThreadSync.hpp"


#define OAK_DECLARE_HEAP \
    public: \
        static Oak::Void * operator new(Oak::SizeT size); \
        static Oak::Void operator delete(Oak::Void * p, Oak::SizeT size); \
    private: \
        static Oak::Core::Heap * s_pHeap;


#define OAK_DEFINE_HIERARCHICALHEAP(className, heapName, parentHeapName) \
    Oak::Core::Heap * className::s_pHeap = nullptr; \
    Oak::Void * className::operator new(Oak::SizeT size) \
    { \
        if (s_pHeap == nullptr)  \
            s_pHeap = Oak::Core::HeapFactory::CreateHeap(heapName, parentName); \
        return ::operator new(size, s_pHeap); \
    } \
    Oak::Void className::operator delete(Oak::Void * p, Oak::SizeT) \
    { \
        ::operator delete(p); \
    }


#define OAK_DEFINE_HEAP(className, heapName) \
    Oak::Core::Heap * className::s_pHeap = nullptr; \
    Oak::Void * className::operator new(Oak::SizeT size) \
    { \
        if (s_pHeap == nullptr)  \
            s_pHeap = Oak::Core::HeapFactory::CreateHeap(heapName); \
        return ::operator new(size, s_pHeap); \
    } \
    Oak::Void className::operator delete(Oak::Void * p, Oak::SizeT) \
    { \
        ::operator delete(p); \
    }


namespace Oak {
namespace Core {


struct IAllocator
{
    virtual Void* Allocate(SizeT bytes, SizeT alignment) = 0;

    virtual Void Deallocate(Void* pAddress) = 0;
};


struct AllocHeader;

class Heap
{
public:
    static constexpr SizeT DEFAULT_ALIGNMENT_SIZE = sizeof(UInt64);

public:
    Heap() = default;
    ~Heap() = default;

    Void Initialize();

    Void Activate(const Char* name, IAllocator* pAllocator);
    Void Deactivate();

    Void* Allocate(SizeT bytes, SizeT alignment = DEFAULT_ALIGNMENT_SIZE);
    Void* AllocateDebug(const Char* file, Int32 line, SizeT bytes, SizeT alignment = DEFAULT_ALIGNMENT_SIZE);

    static Void Deallocate(Void* pAddress);

    Void AttachTo(Heap * pParent);

    Bool IsActive() const { return m_isActive; }
    const Char* GetHeapName() const { return m_heapName; }

    Void PrintTreeInfo(UInt32 indentLevel = 0) const;
    Void PrintInfo(UInt32 indentLevel = 0) const;

    UInt64 ReportMemoryLeaks(UInt64 bookmarkBegin, UInt64 bookmarkEnd) const;

    static UInt64 GetMemoryBookmark();

private:
    Void Deallocate(AllocHeader* pAllocHeader);
    Void GetTreeStats(UInt64& totalBytes, UInt64& totalPeak, UInt64& totalInstances) const;

    IAllocator* m_pAllocator;

    enum { NAMELENGTH = 128 };

    Bool            m_isActive;
    Char            m_heapName[NAMELENGTH];
    UInt64          m_bytesAllocated;
    UInt64          m_bytesAllocatedPeak;
    UInt64          m_instancesAllocated;
    AllocHeader*    m_pAllocHeader;

    Heap * m_pParent;
    Heap * m_pFirstChild;
    Heap * m_pNextSibling;
    Heap * m_pPrevSibling;

    static Core::Mutex  m_heapProtection;
    static UInt64       m_nextMemoryBookmark;
};



} // namespace Core
} // namespace Oak


