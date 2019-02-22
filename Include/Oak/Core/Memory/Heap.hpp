
#pragma once

#include "Oak/Platform/Thread/CriticalSection.hpp"
#include "Oak/Platform/Thread/LockGuard.hpp"
#include "Oak/Core/Memory/MemoryTracker.hpp"
#include "Oak/Core/Assert.hpp"
#include <typeinfo>

namespace Oak
{

class IMemoryLeakReporter;
class IHeapTreeStatsReporter;
class IMemoryAssertionReporter;

typedef Int32 AllocationSignature;

struct Allocation
{
    static constexpr AllocationSignature SIGNATURE = 0xCDCDCDCD;

    Void* pBlock;                    // アドレス
    SizeT bytes;                     // 確保サイズ
    UInt32 pool;                     // 確保したメモリプール
    const Char* file;                // ファイル名
    Int32 line;                      // 行数
    const Char* function;            // 関数名
    TimeT time;                      // 確保日時
    UInt64 backTraceHash;            // バックトレースのハッシュ値
    AllocationSignature* pSignature; // メモリ破壊チェック用
    UInt64 bookmark;                 // ブックマーク
    Heap* pHeap;                     // 確保したヒープ領域

    Allocation* pNext; // リンクリスト (ヒープをウォークするのに必要)
    Allocation* pPrev;
};

namespace Detail
{

class HeapAllocatorBase
{
public:
    virtual const SizeT GetHash() const = 0;

    virtual Void* AllocateBytes(SizeT bytes) = 0;

    virtual Void* AllocateBytesAligned(SizeT bytes, SizeT alignment) = 0;

    virtual Void DeallocateBytes(Void* pBlock) = 0;

    virtual Void DeallocateBytesAligned(Void* pBlock, SizeT alignment) = 0;
};

template <typename Policy>
class PolicyHashCode
{
public:
    static const SizeT value;
};

template <class Policy>
const SizeT PolicyHashCode<Policy>::value =
  typeid(const PolicyHashCode*).hash_code();

template <typename Policy>
class HeapAllocatorOverride : public HeapAllocatorBase
{
public:
    typedef Policy PolicyType;

    static inline HeapAllocatorBase* Get()
    {
        static HeapAllocatorOverride instance;
        return &instance;
    }

    inline const SizeT GetHash() const override
    {
        return PolicyHashCode<Policy>::value;
    }

    inline Void* AllocateBytes(SizeT bytes) override
    {
        return Policy::AllocateBytes(bytes);
    }

    inline Void* AllocateBytesAligned(SizeT bytes, SizeT alignment) override
    {
        return Policy::AllocateBytesAligned(bytes, alignment);
    }

    inline Void DeallocateBytes(Void* pBlock) override
    {
        Policy::DeallocateBytes(pBlock);
    }

    inline Void DeallocateBytesAligned(Void* pBlock, SizeT alignment) override
    {
        Policy::DeallocateBytesAligned(pBlock, alignment);
    }
};

} // namespace Detail

// ヒープ
class Heap
{
public:
    Heap();

    ~Heap();

    Void Initialize();

    template <typename Policy>
    inline Void Activate(const Char* name);

    Void Deactivate();

    const Char* GetName() const;

    Bool IsActive() const;

    template <typename Policy>
    inline Bool IsActive() const;

    Void* AllocateBytes(SizeT bytes, const Char* file, Int32 line,
                        const Char* function);

    Void* AllocateAlignedBytes(SizeT bytes, SizeT alignment, const Char* file,
                               Int32 line, const Char* function);

    Void DeallocateBytes(Void* pBlock);

    Void DeallocateAlignedBytes(Void* pBlock, SizeT alignment);

    // リンクリストを構築
    Void AddAllocation(Allocation* pAllocation);

    // リンクリストから切り離す
    Void EraseAllocation(Allocation* pAllocation);

    // 親子関係の構築をする関数
    Void AttachTo(Heap* pParent);

    // リークのチェック関数
    Void MemoryLeakCheck(IMemoryLeakReporter* pReporter, UInt64 bookmarkStart,
                         UInt64 bookmarkEnd) const;

    // 情報収集のための関数
    Void ReportTreeStats(IHeapTreeStatsReporter* pAccumulator,
                         Int32 depth) const;

    // メモリ破壊のチェック関数
    Void MemoryAssertionCheck(IMemoryAssertionReporter* pReporter,
                              UInt64 bookmarkStart, UInt64 bookmarkEnd) const;

protected:
    Void GetTreeStats(SizeT& totalBytes, SizeT& totalPeakBytes,
                      SizeT& totalInstanceCount) const;

private:
    CriticalSection m_protection;

    enum
    {
        NAMELENGTH = 128
    };
    Char m_name[NAMELENGTH];

    SizeT m_totalAllocatedBytes;
    SizeT m_peakAllocatedBytes;
    SizeT m_allocatedInstanceCount;
    Allocation* m_pAllocation; // リンクリスト

    Heap* m_pParent;
    Heap* m_pFirstChild;
    Heap* m_pNextSibling;
    Heap* m_pPrevSibling;

    Bool m_isActive;

    Detail::HeapAllocatorBase* m_pBaseAllocator;
};

template <typename Policy>
inline Void Heap::Activate(const Char* name)
{
    LockGuard<CriticalSection> lock(m_protection);

    OAK_ASSERT(name != nullptr);
    OAK_ASSERT(strlen(name) < NAMELENGTH);
    strcpy_s(m_name, name);
    m_isActive = true;
    m_totalAllocatedBytes = 0;
    m_peakAllocatedBytes = 0;
    m_allocatedInstanceCount = 0;

    m_pBaseAllocator = Detail::HeapAllocatorOverride<Policy>::Get();
}

template <typename Policy>
inline Bool Heap::IsActive() const
{
    return m_pBaseAllocator != nullptr &&
           Detail::PolicyHashCode<Policy>::value ==
             m_pBaseAllocator->GetHash() &&
           m_isActive;
}

} // namespace Oak
