#include "Heap.hpp"
#include "Assert.hpp"

#include <string.h>


namespace Oak {
namespace Memory {


struct AllocHeader
{
    time_t      time;           // 確保時刻
    UInt64      bytes;          // サイズ
    UInt64      bookmark;       // ブックマーク
    UInt64      stackTraceHash; // スタックトレースハッシュ
    UInt64      line;           // 行数
    const Char* file;           // ファイル名
    UInt64*     pBeginTrap;     // 開始トラップ
    UInt64*     pEndTrap;       // 末尾トラップ
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

}

Void* Heap::AllocateDebug(const Char* file, Int32 line, SizeT bytes, SizeT alignment)
{

}

Void Heap::Deallocate(Void* pAddress)
{

}

Void Heap::AttachTo(Heap * pParent)
{

}

Void Heap::PrintTreeInfo(UInt32 indentLevel) const
{

}

Void Heap::PrintInfo(UInt32 indentLevel) const
{

}

UInt64 Heap::ReportMemoryLeaks(UInt64 bookmarkBegin, UInt64 bookmarkEnd) const
{

}

UInt64 Heap::GetMemoryBookmark()
{

}

Void Heap::Deallocate(AllocHeader* pAllocHeader)
{

}

Void Heap::GetTreeStats(UInt64& totalBytes, UInt64& totalPeak, UInt64& totalInstances) const
{

}





} // namespace Memory
} // namespace Oak




