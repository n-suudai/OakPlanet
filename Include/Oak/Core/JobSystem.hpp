
#pragma once

#include "Oak/Core/Assert.hpp"
#include "Oak/Core/STL.hpp"
#include "Oak/Platform/Thread/Thread.hpp"
#include "Oak/Platform/Thread/LockGuard.hpp"

namespace Oak
{

typedef Void (*JobCallback)(Void* pArgumentBlock, SizeT argumentSize);

struct Job
{
    OAK_DECLARE_HEAP;

    Void Execute()
    {
        if (function != nullptr)
        {
            function(pArgumentBlock, argumentSize);
        }
    }

    JobCallback function;
    Void* pArgumentBlock;
    SizeT argumentSize;
};

class JobThread
{
    OAK_DECLARE_HEAP;

public:
    JobThread(Fiber* pOwner)
      : m_pOwnerFiber(pOwner)
      , m_thread("JobThread", &JobThread::Execute)
      , m_isTerminateRequested(false)
      , m_isActive(false)
    {
        OAK_ASSERT(m_pOwnerFiber != nullptr);

        m_thread.Start(this, sizeof(this));
    }

    ~JobThread()
    {
        m_isTerminateRequested = true;
        m_thread.Wait();
    }

    Void BeStart()
    {
        m_isActive = true;
    }

    Bool IsBusy() const
    {
        return m_isActive;
    }

    Void Execute()
    {
        while (!m_isTerminateRequested)
        {
            Thread::Sleep(2);
            if (m_isActive)
            {
                Job* pJob = m_pOwnerFiber->GetJob();
                while (pJob != nullptr)
                {
                    pJob->Execute();
                    pJob = m_pOwnerFiber->GetJob();
                }
                m_isActive = false;
            }
        }
    }

private:
    static UInt32 Execute(Void* pBlock, SizeT blockSize)
    {
        OAK_ASSERT(pBlock != nullptr);

        JobThread* pSelf = reinterpret_cast<JobThread*>(pBlock);

        pSelf->Execute();

        return 0;
    }

private:
    Fiber* m_pOwnerFiber;
    Thread m_thread;
    Bool m_isTerminateRequested;
    Bool m_isActive;
};

class Fiber
{
    OAK_DECLARE_HEAP;

public:
    Fiber(SizeT threadCount) : m_jobThreads(threadCount), m_jobPointer(0)
    {
        for (auto& jobThread : m_jobThreads)
        {
            jobThread = OAK_NEW JobThread(this);
        }
    }

    ~Fiber()
    {
        for (auto& jobThread : m_jobThreads)
        {
            OAK_DELETE jobThread;
        }
    }

    Job* GetJob()
    {
        LockGuard<CriticalSection> lock(m_protection);
        if (m_jobPointer < m_jobList.size())
        {
            Job* pJob = &m_jobList[m_jobPointer++];
            return pJob;
        }
        return nullptr;
    }

    Void Execute()
    {
        for (auto& jobThread : m_jobThreads)
        {
            jobThread->BeStart();
        }

        Bool isBusy = true;
        while (isBusy)
        {
            Thread::Sleep(2);
            isBusy = false;
            for (auto& jobThread : m_jobThreads)
            {
                isBusy = isBusy || jobThread->IsBusy();
            }
        }

        m_jobList.clear();
        m_jobPointer = 0;
    }

    Void AddJob(const Job& job)
    {
        LockGuard<CriticalSection> lock(m_protection);
        m_jobList.push_back(job);
    }

private:
    CriticalSection m_protection;
    STL<>::deque<Job> m_jobList;
    STL<>::vector<JobThread*> m_jobThreads;
    SizeT m_jobPointer;
};

} // namespace Oak
