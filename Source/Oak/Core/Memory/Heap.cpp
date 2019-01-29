#include "Oak/Core/Memory/Heap.hpp"
#include "Oak/Core/Assert.hpp"
#include "Oak/Core/Log.hpp"

#include <string.h>




namespace Oak {
namespace Core {


constexpr UInt32 OAK_MEMORY_TRAP = 0xCDCDCDCD;


struct AllocHeader
{
    time_t      time;           // 確保時刻
    UInt64      bytes;          // サイズ
    UInt64      bookmark;       // ブックマーク
    UInt64      stackTraceHash; // スタックトレースハッシュ
    UInt64      line;           // 行数
    const Char* file;           // ファイル名
    Heap*       pHeap;          // ヒープ
    UInt32*     pBeginTrap;     // 開始トラップ
    UInt32*     pEndTrap;       // 末尾トラップ
};


UInt64 Heap::m_nextMemoryBookmark = 1;


Void Heap::Initialize()
{
    m_pAllocator = nullptr;

    m_isActive = false;
    strcpy_s(m_heapName, "");
    m_bytesAllocated = 0;
    m_bytesAllocatedPeak = 0;
    m_instancesAllocated = 0;
    m_pAllocHeader = nullptr;

    m_pParent = nullptr;
    m_pFirstChild = nullptr;
    m_pNextSibling = nullptr;
    m_pPrevSibling = nullptr;
}

Void Heap::Activate(const Char* name, IAllocator* pAllocator)
{
    OAK_ASSERT(name != nullptr);
    OAK_ASSERT(strlen(name) < NAMELENGTH);
    OAK_ASSERT(pAllocator != nullptr);

    m_pAllocator = pAllocator;

    strcpy_s(m_heapName, name);

    m_bytesAllocated = 0;
    m_bytesAllocatedPeak = 0;
    m_instancesAllocated = 0;

    m_isActive = true;
}

Void Heap::Deactivate()
{
    m_pAllocator = nullptr;

    strcpy_s(m_heapName, "");

    m_bytesAllocated = 0;
    m_bytesAllocatedPeak = 0;
    m_instancesAllocated = 0;

    m_isActive = false;
}

Void* Heap::Allocate(SizeT bytes, SizeT alignment)
{
    OAK_ASSERT(m_pAllocator != nullptr);

    return m_pAllocator->Allocate(bytes, alignment);
}

Void* Heap::AllocateDebug(const Char* file, Int32 line, SizeT bytes, SizeT alignment)
{

}

Void Heap::Deallocate(Void* pAddress)
{
    //AllocHeader * pHeader = (AllocHeader *)((PtrDiff)pAddress - sizeof(AllocHeader));
    //OAK_ASSERT((*pHeader->pBeginTrap) == OAK_MEMORY_TRAP);
    //pHeader->pHeap->Deallocate(pHeader);
}

Void Heap::AttachTo(Heap * pParent)
{

}

Void Heap::PrintTreeInfo(UInt32 indentLevel) const
{
    PrintInfo(indentLevel);
    Heap * pChild = m_pFirstChild;
    while (pChild != nullptr)
    {
        pChild->PrintTreeInfo(indentLevel + 1);
        pChild = pChild->m_pNextSibling;
    }
}

Void Heap::PrintInfo(UInt32 indentLevel) const
{
    for (UInt32 i = 0; i < indentLevel; ++i)
    {
        printf("  ");
    }

    UInt64 totalBytes = 0;
    UInt64 totalPeakBytes = 0;
    UInt64 totalInstances = 0;
    GetTreeStats(totalBytes, totalPeakBytes, totalInstances);

    UInt32 spacing = 20 - indentLevel * 2;
    printf("%-*s %6d %6d %5d  %6d %6d %5d\n",
        spacing, m_heapName,
        m_bytesAllocated, m_bytesAllocatedPeak, m_instancesAllocated,
        totalBytes, totalPeakBytes, totalInstances);
}

UInt64 Heap::ReportMemoryLeaks(UInt64 bookmarkBegin, UInt64 bookmarkEnd) const
{

}

UInt64 Heap::GetMemoryBookmark()
{
    return m_nextMemoryBookmark;
}

Void Heap::Deallocate(AllocHeader* pAllocHeader)
{

}

Void Heap::GetTreeStats(UInt64& totalBytes, UInt64& totalPeak, UInt64& totalInstances) const
{
    totalBytes += m_bytesAllocated;
    totalPeak += m_bytesAllocatedPeak;
    totalInstances += m_instancesAllocated;

    Heap * pChild = m_pFirstChild;
    while (pChild != nullptr)
    {
        pChild->GetTreeStats(totalBytes, totalPeak, totalInstances);
        pChild = pChild->m_pNextSibling;
    }
}





} // namespace Core
} // namespace Oak




