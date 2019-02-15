
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"
#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Platform/Thread/Mutex.hpp"
#include "Oak/Platform/Thread/LockGuard.hpp"


// default global new does not tracking
#include <unordered_map>
#include <string>


#if OAK_MEMORY_TRACKER


namespace  Oak {


class MemoryTracker
{
protected:
    struct Allocation
    {
        Void*       pBlock;
        SizeT       bytes;
        UInt32      pool;
        std::string file;
        Int32       line;
        std::string func;
        TimeT       time;
        UInt64      backTraceHash;

        inline Allocation()
        {
            pBlock = nullptr;
            bytes = 0;
            pool = 0;
            file = "";
            line = 0;
            func = "";
            time = 0;
            backTraceHash = 0;
        }

        inline Allocation(
            Void* pBlock,
            SizeT bytes,
            UInt32 pool,
            const Char* file,
            Int32 line,
            const Char* func,
            TimeT time,
            UInt64 backTraceHash
        )
        {
            this->pBlock = pBlock;
            this->bytes = bytes;
            this->pool = pool;
            this->file = file;
            this->line = line;
            this->func = func;
            this->time = time;
            this->backTraceHash = backTraceHash;
        }
    };

    Mutex m_protection;
    std::unordered_map<SizeT, Allocation> m_allocations; // key : bookmark
    std::unordered_map<Void*, SizeT> m_allocationBookmarks;

    SizeT m_nextAllocationBookmark;
    SizeT m_totalAllocatedBytes;
    SizeT m_peakAllocatedBytes;

public:
    inline MemoryTracker()
        : m_protection("MemoryTracker")
        , m_allocations()
        , m_allocationBookmarks()
        , m_nextAllocationBookmark(1)
        , m_totalAllocatedBytes(0)
        , m_peakAllocatedBytes(0)
    {

    }

    inline ~MemoryTracker() {}

    inline Void RecordAllocation(
        Void* pBlock,
        SizeT bytes,
        UInt32 pool,
        const Char* file,
        Int32 line,
        const Char* func
    )
    {
        LockGuard<Mutex> lock(m_protection);

        // TODO : checking double allocation

        m_allocationBookmarks[pBlock] = m_nextAllocationBookmark;
        m_allocations[m_nextAllocationBookmark] = Allocation(
            pBlock,
            bytes,
            pool,
            file,
            line,
            func,
            0,
            0
        );

        m_totalAllocatedBytes += bytes;

        if (m_totalAllocatedBytes > m_peakAllocatedBytes)
        {
            m_peakAllocatedBytes = m_totalAllocatedBytes;
        }

        m_nextAllocationBookmark++;
    }

    inline Void RecordDeallocation(Void* pBlock)
    {
        LockGuard<Mutex> lock(m_protection);

        // TODO : checking double deallocation

        SizeT bookmark = m_allocationBookmarks[pBlock];
        
        m_totalAllocatedBytes -= m_allocations[bookmark].bytes;

        m_allocations.erase(bookmark);

        m_allocationBookmarks.erase(pBlock);
    }

    inline SizeT GetTotalMemoryAllocatedBytes() const
    {
        return m_totalAllocatedBytes;
    }

    inline SizeT GetPeakMemoryAllocatedBytes() const
    {
        return m_peakAllocatedBytes;
    }

    inline SizeT GetAliveInstanceCount() const
    {
        return m_allocationBookmarks.size();
    }

    // TODO : implement report memory leaks function

    static inline MemoryTracker& Get()
    {
        static MemoryTracker instance;
        return instance;
    }
};


} // namespace Oak


#endif  // OAK_MEMORY_TRACKER

