
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"
#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Platform/Thread/CriticalSection.hpp"
#include "Oak/Platform/Thread/LockGuard.hpp"
#include "Oak/Core/Assert.hpp"


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
        const Char* file;
        Int32       line;
        const Char* func;
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

    CriticalSection m_protection;
    std::unordered_map<SizeT, Allocation> m_allocations; // key : bookmark
    std::unordered_map<Void*, SizeT> m_allocationBookmarks;

    SizeT m_nextAllocationBookmark;
    SizeT m_totalAllocatedBytes;
    SizeT m_peakAllocatedBytes;

public:
    inline MemoryTracker()
        : m_protection()
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
        LockGuard<CriticalSection> lock(m_protection);

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
        LockGuard<CriticalSection> lock(m_protection);

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









typedef Int32 AllocationSignature;
class Heap;

struct Allocation
{
    static constexpr AllocationSignature SIGNATURE = 0xCDCDCDCD;

    Void*                   pBlock;         // アドレス
    SizeT                   bytes;          // 確保サイズ
    UInt32                  pool;           // 確保したメモリプール
    const Char*             file;           // ファイル名
    Int32                   line;           // 行数
    const Char*             function;       // 関数名
    TimeT                   time;           // 確保日時
    UInt64                  backTraceHash;  // バックトレースのハッシュ値
    AllocationSignature*    pSignature;     // メモリ破壊チェック用
    UInt64                  bookmark;       // ブックマーク
    Heap*                   pHeap;          // 確保したヒープ領域

    Allocation* pNext;      // リンクリスト (ヒープをウォークするのに必要)
    Allocation* pPrev;
};


class Tracker
{
public:
    static inline Tracker& Get()
    {
        static Tracker instance;
        return instance;
    }

    inline Tracker()
        : m_protection()
        , m_allocations()
        , m_nextAllocationBookmark(1)
    {

    }

    inline ~Tracker()
    {
        // ※この時点で残っているAllocationはリークしている
    }

    inline Allocation* RecordAllocation(
        Void* pBlock,
        SizeT bytes,
        const Char* file,
        Int32 line,
        const Char* function,
        Heap* pHeap
    );

    inline Void RecordDeallocation(Void* pBlock, Heap* pHeap);

    inline SizeT GetAllocationBookmark()
    {
        return m_nextAllocationBookmark;
    }

private:
    CriticalSection m_protection;
    std::unordered_map<Void*, Allocation*> m_allocations;
    SizeT m_nextAllocationBookmark;
};

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

        inline HeapTreeStats()
            : totalBytes(0)
            , peakBytes(0)
            , instanceCount(0)
        {}
    };

    virtual ~IHeapTreeStatsReporter() = default;

    virtual Void BeginReport() = 0;

    virtual Void Report(Int32 depth, const Heap* pHeap, const HeapTreeStats& local, const HeapTreeStats& total) = 0;

    virtual Void EndReport() = 0;
};

// メモリ破壊
class IMemoryAssertionReporter
{
public:
    virtual ~IMemoryAssertionReporter() = default;

    virtual Void BeginReport() = 0;

    virtual Void Report(const Heap* pHeap, const Allocation* pAllocation) = 0;

    virtual Void EndReport() = 0;
};


// ヒープ
class Heap
{
public:
    inline Heap()
        : m_protection()
        , m_totalAllocatedBytes(0)
        , m_peakAllocatedBytes(0)
        , m_allocatedInstanceCount(0)
        , m_pAllocation(nullptr)
        , m_pParent(nullptr)
        , m_pFirstChild(nullptr)
        , m_pNextSibling(nullptr)
        , m_pPrevSibling(nullptr)
        , m_isActive(false)
    {

    }

    inline ~Heap()
    {

    }

    inline Void Initialize()
    {
        LockGuard<CriticalSection> lock(m_protection);

        m_isActive = false;
        m_totalAllocatedBytes = 0;
        m_peakAllocatedBytes = 0;
        m_allocatedInstanceCount = 0;
        m_pAllocation = nullptr;
        m_pParent = nullptr;
        m_pFirstChild = nullptr;
        m_pNextSibling = nullptr;
        m_pPrevSibling = nullptr;
    }

    inline Void Activate(const Char* name)
    {
        LockGuard<CriticalSection> lock(m_protection);

        OAK_ASSERT(name != nullptr);
        OAK_ASSERT(strlen(name) < NAMELENGTH);
        strcpy_s(m_name, name);
        m_isActive = true;
        m_totalAllocatedBytes = 0;
        m_peakAllocatedBytes = 0;
        m_allocatedInstanceCount = 0;
    }

    inline Void Deactivate()
    {
        LockGuard<CriticalSection> lock(m_protection);

        strcpy_s(m_name, "");
        m_isActive = false;
        m_totalAllocatedBytes = 0;
        m_peakAllocatedBytes = 0;
        m_allocatedInstanceCount = 0;
    }

    inline const Char* GetName() const
    {
        return m_name;
    }

    inline Bool IsActive() const
    {
        return m_isActive;
    }

    template<typename Policy>
    inline Void* Allocate(SizeT bytes, const Char* file, Int32 line, const Char* function)
    {
        LockGuard<CriticalSection> lock(m_protection);

        // シグネチャサイズをプラス
        constexpr SizeT signatureSize = sizeof(AllocationSignature);

        // ポリシーを利用してメモリを確保
        Void* pBlock = Policy::AllocateBytes(bytes + signatureSize, file, line, function);

        // トラッカーへ情報を登録
        Tracker::Get().RecordAllocation(
            pBlock,
            bytes,
            file,
            line,
            function,
            this
        );

        return pBlock;
    }

    template<typename Policy>
    inline Void* AllocateAligned(SizeT bytes, SizeT alignment, const Char* file, Int32 line, const Char* function)
    {
        LockGuard<CriticalSection> lock(m_protection);

        // シグネチャサイズをプラス
        constexpr SizeT signatureSize = sizeof(AllocationSignature);

        // ポリシーを利用してメモリを確保
        Void* pBlock = Policy::AllocateAlignedBytes(bytes + signatureSize, alignment, file, line, function);

        // トラッカーへ情報を登録
        Tracker::Get().RecordAllocation(
            pBlock,
            bytes,
            file,
            line,
            function,
            this
        );

        return pBlock;
    }

    template<typename Policy>
    inline Void Deallocate(Void* pBlock)
    {
        LockGuard<CriticalSection> lock(m_protection);

        // トラッカーから情報を削除
        Tracker::Get().RecordDeallocation(pBlock, this);

        // ポリシーを利用してメモリを破棄
        Policy::DeallocateBytes(pBlock);
    }

    template<typename Policy>
    inline Void DeallocateAligned(Void* pBlock)
    {
        LockGuard<CriticalSection> lock(m_protection);

        // トラッカーから情報を削除
        Tracker::Get().RecordDeallocation(pBlock, this);

        // ポリシーを利用してメモリを破棄
        Policy::DeallocateAlignedBytes(pBlock);
    }

    // リンクリストを構築
    inline Void AddAllocation(Allocation* pAllocation)
    {
        LockGuard<CriticalSection> lock(m_protection);

        pAllocation->pPrev = nullptr;
        pAllocation->pNext = m_pAllocation;

        if (m_pAllocation != nullptr)
        {
            m_pAllocation->pPrev = pAllocation;
        }

        m_pAllocation = pAllocation;

        // 確保サイズ情報更新
        m_totalAllocatedBytes += pAllocation->bytes;

        if (m_totalAllocatedBytes > m_peakAllocatedBytes)
        {
            m_peakAllocatedBytes = m_totalAllocatedBytes;
        }

        m_allocatedInstanceCount++;
    }

    // リンクリストから切り離す
    inline Void EraseAllocation(Allocation* pAllocation)
    {
        LockGuard<CriticalSection> lock(m_protection);

        if (pAllocation->pPrev == nullptr)
        {
            OAK_ASSERT(pAllocation == m_pAllocation);
            m_pAllocation = pAllocation->pNext;
        }
        else
        {
            pAllocation->pPrev->pNext = pAllocation->pNext;
        }

        if (pAllocation->pNext != nullptr)
        {
            pAllocation->pNext->pPrev = pAllocation->pPrev;
        }

        // 確保サイズ情報更新
        m_totalAllocatedBytes -= pAllocation->bytes;
        m_allocatedInstanceCount--;
    }

    // 親子関係の構築をする関数
    inline Void AttachTo(Heap* pParent)
    {
        LockGuard<CriticalSection> lock(m_protection);

        OAK_ASSERT(pParent != nullptr);

        if (pParent == m_pParent) { return; }

        // 1. 現在の親子関係を切り離す
        if (m_pPrevSibling != nullptr)
        {
            m_pPrevSibling->m_pNextSibling = m_pNextSibling;
        }

        if (m_pNextSibling != nullptr)
        {
            m_pNextSibling->m_pPrevSibling = m_pPrevSibling;
        }

        if (m_pParent != nullptr)
        {
            if (m_pParent->m_pFirstChild == this)
            {
                m_pParent->m_pFirstChild = m_pNextSibling;
            }
        }

        // 2. 新しく親子関係を構築
        m_pNextSibling = pParent->m_pFirstChild;
        m_pPrevSibling = nullptr;
        m_pParent = pParent;
        pParent->m_pFirstChild = this;
    }

    // リークのチェック関数
    inline Void MemoryLeakCheck(IMemoryLeakReporter* pReporter, UInt64 bookmarkStart, UInt64 bookmarkEnd) const
    {
        OAK_ASSERT(pReporter != nullptr);

        Allocation* pAllocation = m_pAllocation;
        while (pAllocation != nullptr)
        {
            if (pAllocation->bookmark >= bookmarkStart && pAllocation->bookmark <= bookmarkEnd)
            {
                pReporter->Report(this, pAllocation);
            }
            pAllocation = pAllocation->pNext;
        }
    }

    // 情報収集のための関数
    inline Void ReportTreeStats(IHeapTreeStatsReporter* pAccumulator, Int32 depth) const
    {
        OAK_ASSERT(pAccumulator != nullptr);

        IHeapTreeStatsReporter::HeapTreeStats local, total;

        local.totalBytes = m_totalAllocatedBytes;
        local.peakBytes = m_peakAllocatedBytes;
        local.instanceCount = m_allocatedInstanceCount;

        GetTreeStats(total.totalBytes, total.peakBytes, total.instanceCount);

        pAccumulator->Report(depth, this, local, total);

        Heap* pChild = m_pFirstChild;
        while (pChild != nullptr)
        {
            pChild->ReportTreeStats(pAccumulator, depth + 1);
            pChild = pChild->m_pNextSibling;
        }
    }

    // メモリ破壊のチェック関数
    inline Void MemoryAssertionCheck(IMemoryAssertionReporter* pReporter, UInt64 bookmarkStart, UInt64 bookmarkEnd) const
    {
        OAK_ASSERT(pReporter != nullptr);

        Allocation* pAllocation = m_pAllocation;
        while (pAllocation != nullptr)
        {
            if (pAllocation->bookmark >= bookmarkStart && pAllocation->bookmark <= bookmarkEnd)
            {
                if ((*pAllocation->pSignature) != Allocation::SIGNATURE)
                {
                    pReporter->Report(this, pAllocation);
                }
            }
            pAllocation = pAllocation->pNext;
        }
    }

protected:
    inline Void GetTreeStats(SizeT& totalBytes, SizeT& totalPeakBytes, SizeT& totalInstanceCount) const
    {
        totalBytes += m_totalAllocatedBytes;
        totalPeakBytes += m_peakAllocatedBytes;
        totalInstanceCount += m_allocatedInstanceCount;

        Heap * pChild = m_pFirstChild;
        while (pChild != NULL)
        {
            pChild->GetTreeStats(totalBytes, totalPeakBytes, totalInstanceCount);
            pChild = pChild->m_pNextSibling;
        }
    }

private:
    CriticalSection m_protection;

    enum { NAMELENGTH = 128 };
    Char m_name[NAMELENGTH];

    SizeT m_totalAllocatedBytes;
    SizeT m_peakAllocatedBytes;
    SizeT m_allocatedInstanceCount;
    Allocation* m_pAllocation;  // リンクリスト

    Heap* m_pParent;
    Heap* m_pFirstChild;
    Heap* m_pNextSibling;
    Heap* m_pPrevSibling;

    Bool m_isActive;
};


inline Allocation* Tracker::RecordAllocation(
    Void* pBlock,
    SizeT bytes,
    const Char* file,
    Int32 line,
    const Char* function,
    Heap* pHeap
)
{
    LockGuard<CriticalSection> lock(m_protection);

    Allocation* pAllocation = new Allocation(); // デフォルトの new
    pAllocation->pBlock = pBlock;
    pAllocation->bytes = bytes;
    pAllocation->file = file;
    pAllocation->line = line;
    pAllocation->function = function;
    pAllocation->time = 0;
    pAllocation->backTraceHash = 0;
    pAllocation->pSignature = reinterpret_cast<AllocationSignature*>(reinterpret_cast<PtrDiff>(pBlock) + bytes);
    pAllocation->bookmark = m_nextAllocationBookmark;
    pAllocation->pHeap = pHeap;

    // シグネチャを書き込み
    (*pAllocation->pSignature) = Allocation::SIGNATURE;

    m_allocations[pBlock] = pAllocation;

    // リンクリストを構築
    pHeap->AddAllocation(pAllocation);

    m_nextAllocationBookmark++;

    return pAllocation;
}

inline Void Tracker::RecordDeallocation(Void* pBlock, Heap* pHeap)
{
    LockGuard<CriticalSection> lock(m_protection);

    auto it = m_allocations.find(pBlock);

    if (it != m_allocations.end())
    {
        // シグネチャをチェック
        OAK_ASSERT((*it->second->pSignature) == Allocation::SIGNATURE);

        // リンクリストから切り離す
        pHeap->EraseAllocation(it->second);

        delete it->second;  // デフォルトの delete

        m_allocations.erase(it);
    }
}


class HeapFactory
{
public:
    static inline HeapFactory& Get()
    {
        static HeapFactory instance;
        return instance;
    }

    inline HeapFactory()
    {
        Initialize();
    }

    inline ~HeapFactory()
    {

    }

    inline Void Initialize()
    {
        LockGuard<CriticalSection> lock(m_protection);

        for (auto& heap : m_heaps)
        {
            heap.Initialize();
        }

        m_pRootHeap = CreateNewHeap(s_pRootHeapName);
        m_pDefaultHeap = CreateHeap(s_pDefaultHeapName);
    }

    inline Heap* GetRootHeap()
    {
        LockGuard<CriticalSection> lock(m_protection);

        if (m_pRootHeap == nullptr)
        {
            Initialize();
        }

        return m_pRootHeap;
    }

    inline Heap* GetDefaultHeap()
    {
        LockGuard<CriticalSection> lock(m_protection);

        if (m_pDefaultHeap == nullptr)
        {
            Initialize();
        }

        return m_pDefaultHeap;
    }

    inline Heap* CreateHeap(const Char* name, const Char* parentName)
    {
        LockGuard<CriticalSection> lock(m_protection);

        if (m_pRootHeap == nullptr)
        {
            Initialize();
        }

        // 親ヒープを探す
        Heap* pParent = FindHeap(parentName);
        if (pParent == nullptr)
        { // 見つからなければ Root 以下に作成
            pParent = CreateNewHeap(parentName);
            pParent->AttachTo(m_pRootHeap);
        }

        // ヒープを探す
        Heap* pHeap = FindHeap(name);
        if (pHeap == nullptr)
        { // 見つからなければ作成
            pHeap = CreateNewHeap(name);
        }

        // pParent以下にアタッチ
        pHeap->AttachTo(pParent);

        return pHeap;
    }

    inline Heap* CreateHeap(const Char* name)
    {
        return CreateHeap(name, s_pRootHeapName);
    }

    inline Heap* FindHeap(const Char* name)
    {
        LockGuard<CriticalSection> lock(m_protection);

        for (auto& heap : m_heaps)
        {
            if (heap.IsActive() && !_stricmp(name, heap.GetName()))
            {
                return &heap;
            }
        }

        return nullptr;
    }

    inline Heap* CreateNewHeap(const Char* name)
    {
        LockGuard<CriticalSection> lock(m_protection);

        for (auto& heap : m_heaps)
        {
            if (!heap.IsActive())
            {
                heap.Activate(name);
                return &heap;
            }
        }

        OAK_ASSERT_MESSAGE(false, "ヒープの作成に失敗しました。作成可能なヒープの数を超えています。");
        return nullptr;
    }


    // リークのチェック関数
    inline Void MemoryLeakCheck(IMemoryLeakReporter* pReporter, UInt64 bookmarkStart, UInt64 bookmarkEnd) const
    {
        OAK_ASSERT(pReporter != nullptr);

        pReporter->BeginReport();

        for (auto& heap : m_heaps)
        {
            if (heap.IsActive())
            {
                heap.MemoryLeakCheck(pReporter, bookmarkStart, bookmarkEnd);
            }
        }

        pReporter->EndReport();
    }

    inline Void MemoryLeakCheck(IMemoryLeakReporter* pReporter, UInt64 bookmarkStart = 0)
    {
        MemoryLeakCheck(pReporter, bookmarkStart, Tracker::Get().GetAllocationBookmark());
    }

    // メモリ情報
    inline Void ReportHeapTreeStats(IHeapTreeStatsReporter* pReporter)
    {
        OAK_ASSERT(pReporter != nullptr);

        pReporter->BeginReport();

        m_pRootHeap->ReportTreeStats(pReporter, 0);

        pReporter->EndReport();
    }

    // メモリ破壊のチェック関数
    inline Void MemoryAssertionCheck(IMemoryAssertionReporter* pReporter, UInt64 bookmarkStart, UInt64 bookmarkEnd) const
    {
        OAK_ASSERT(pReporter != nullptr);

        pReporter->BeginReport();

        for (auto& heap : m_heaps)
        {
            if (heap.IsActive())
            {
                heap.MemoryAssertionCheck(pReporter, bookmarkStart, bookmarkEnd);
            }
        }

        pReporter->EndReport();
    }

private:
    static constexpr Char* s_pRootHeapName = "Root";
    static constexpr Char* s_pDefaultHeapName = "Default";

    CriticalSection m_protection;

    enum { MAXHEAPS = 256 };
    Heap    m_heaps[MAXHEAPS];
    Heap*   m_pRootHeap;
    Heap*   m_pDefaultHeap;
};


//#define OAK_NEW new(__FILE__, __LINE__, __FUNCTION__)
//#define OAK_DELETE delete


} // namespace Oak



#define OAK_DECLARE_HEAP \
    private: \
        static Oak::Heap* s_pHeap; \
    public: \
        static Oak::Void* operator new (Oak::SizeT bytes, const Oak::Char* file, const Oak::Int32 line, const Oak::Char* function); \
        static Oak::Void* operator new (Oak::SizeT bytes); \
        static Oak::Void operator delete(Oak::Void* pBlock, const Oak::Char* file, const Oak::Int32 line, const Oak::Char* function); \
        static Oak::Void operator delete(Oak::Void* pBlock); \


#define OAK_DEFINE_HEAP(className, heapName, policyName) \
    Oak::Heap* className::s_pHeap = nullptr; \
    Oak::Void* className::operator new (Oak::SizeT bytes, const Oak::Char* file, const Oak::Int32 line, const Oak::Char* function) { \
        if (s_pHeap == nullptr) { s_pHeap = Oak::HeapFactory::Get().CreateHeap(heapName); } \
        return s_pHeap->Allocate<policyName>(bytes, file, line, function); \
    } \
    Oak::Void* className::operator new (Oak::SizeT bytes) { \
        if (s_pHeap == nullptr) { s_pHeap = Oak::HeapFactory::Get().CreateHeap(heapName); } \
        return s_pHeap->Allocate<policyName>(bytes, __FILE__, __LINE__, __FUNCTION__); \
    } \
    Oak::Void className::operator delete(Oak::Void* pBlock, const Oak::Char*, const Oak::Int32, const Oak::Char*) { \
        s_pHeap->Deallocate<policyName>(pBlock); \
    } \
    Oak::Void className::operator delete(Oak::Void* pBlock) { \
        s_pHeap->Deallocate<policyName>(pBlock); \
    }


#define OAK_DEFINE_HIERARCHAL_HEAP(className, heapName, parentName, policyName) \
    Oak::Heap* className::s_pHeap = nullptr; \
    Oak::Void* className::operator new (Oak::SizeT bytes, const Oak::Char* file, const Oak::Int32 line, const Oak::Char* function) { \
        if (s_pHeap == nullptr) { s_pHeap = Oak::HeapFactory::Get().CreateHeap(heapName, parentName); } \
        return s_pHeap->Allocate<policyName>(bytes, file, line, function); \
    } \
    Oak::Void* className::operator new (Oak::SizeT bytes) { \
        if (s_pHeap == nullptr) { s_pHeap = Oak::HeapFactory::Get().CreateHeap(heapName); } \
        return s_pHeap->Allocate<policyName>(bytes, __FILE__, __LINE__, __FUNCTION__); \
    } \
    Oak::Void className::operator delete(Oak::Void* pBlock, const Oak::Char*, const Oak::Int32, const Oak::Char*) { \
        s_pHeap->Deallocate<policyName>(pBlock); \
    } \
    Oak::Void className::operator delete(Oak::Void* pBlock) { \
        s_pHeap->Deallocate<policyName>(pBlock); \
    }


#endif  // OAK_MEMORY_TRACKER

