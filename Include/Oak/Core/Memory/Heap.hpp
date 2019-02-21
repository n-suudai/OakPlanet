
#pragma once

#include "Oak/Platform/Thread/CriticalSection.hpp"
#include "Oak/Platform/Thread/LockGuard.hpp"
#include "Oak/Core/Memory/MemoryTracker.hpp"

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

// ヒープ
class Heap
{
public:
    Heap();

    ~Heap();

    Void Initialize();

    Void Activate(const Char* name);

    Void Deactivate();

    const Char* GetName() const;

    Bool IsActive() const;

    template <typename Policy>
    inline Void* Allocate(SizeT bytes, const Char* file, Int32 line,
                          const Char* function);

    template <typename Policy>
    inline Void Deallocate(Void* pBlock);

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
};

template <typename Policy>
inline Void* Heap::Allocate(SizeT bytes, const Char* file, Int32 line,
                            const Char* function)
{
    LockGuard<CriticalSection> lock(m_protection);

    // シグネチャサイズをプラス
    constexpr SizeT signatureSize = sizeof(AllocationSignature);

    // ポリシーを利用してメモリを確保
    Void* pBlock = Policy::AllocateBytes(bytes + signatureSize);

    // トラッカーへ情報を登録
    MemoryTracker::Get().RecordAllocation(pBlock, bytes, file, line, function,
                                          this);

    return pBlock;
}

template <typename Policy>
inline Void Heap::Deallocate(Void* pBlock)
{
    LockGuard<CriticalSection> lock(m_protection);

    // トラッカーから情報を削除
    MemoryTracker::Get().RecordDeallocation(pBlock, this);

    // ポリシーを利用してメモリを破棄
    Policy::DeallocateBytes(pBlock);
}

} // namespace Oak
