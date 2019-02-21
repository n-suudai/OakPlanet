
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#include "Oak/Platform/Thread/CriticalSection.hpp"

// default global new does not tracking
#include <unordered_map>
#include <string>

namespace Oak
{

class Heap;

struct Allocation;

class MemoryTracker
{
public:
    static MemoryTracker& Get();

    MemoryTracker();

    ~MemoryTracker();

    Allocation* RecordAllocation(Void* pBlock, SizeT bytes, const Char* file,
                                 Int32 line, const Char* function, Heap* pHeap);

    Void RecordDeallocation(Void* pBlock, Heap* pHeap);

    SizeT GetAllocationBookmark() const;

private:
    CriticalSection m_protection;
    std::unordered_map<Void*, Allocation*> m_allocations;
    SizeT m_nextAllocationBookmark;
};

//#define OAK_NEW new(__FILE__, __LINE__, __FUNCTION__)
//#define OAK_DELETE delete

} // namespace Oak
