
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#if OAK_USE_HEAP_TRACKING

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{

class Heap;

struct Allocation;

// メモリリーク
class IMemoryLeakReporter
{
public:
    virtual ~IMemoryLeakReporter() = default;

    virtual Void BeginReport() = 0;

    virtual Void Report(const Heap* pHeap, const Allocation* pAllocation) = 0;

    virtual Void EndReport() = 0;
};

// メモリ情報
class IHeapTreeStatsReporter
{
public:
    struct HeapTreeStats
    {
        SizeT totalBytes;
        SizeT peakBytes;
        SizeT instanceCount;

        inline HeapTreeStats() : totalBytes(0), peakBytes(0), instanceCount(0)
        {
        }
    };

    virtual ~IHeapTreeStatsReporter() = default;

    virtual Void BeginReport() = 0;

    virtual Void Report(Int32 depth, const Heap* pHeap,
                        const HeapTreeStats& local,
                        const HeapTreeStats& total) = 0;

    virtual Void EndReport() = 0;
};

// メモリ破壊
class IMemoryCorruptionReporter
{
public:
    virtual ~IMemoryCorruptionReporter() = default;

    virtual Void BeginReport() = 0;

    virtual Void Report(const Heap* pHeap, const Allocation* pAllocation) = 0;

    virtual Void EndReport() = 0;
};

} // namespace Oak

#endif // OAK_USE_HEAP_TRACKING
