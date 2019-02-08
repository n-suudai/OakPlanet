
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"
#include <malloc.h>


namespace Oak {
namespace Graphics {

typedef Void* (*AllocateFunction)(const Char* filename, UInt64 line, SizeT bytes, SizeT alignment);
typedef Void(*DeallocateFunction)(Void* pBlock);


class IAllocator
{
protected:
    IAllocator() = default;
    virtual ~IAllocator() = default;

public:
    virtual Void* MallocDebug(const Char* filename, UInt64 line, SizeT bytes, SizeT alignment) = 0;
    virtual Void* Malloc(SizeT bytes, SizeT alignment) = 0;

    virtual Void FreeDebug(const Char* filename, UInt64 line, Void* pBlock) = 0;
    virtual Void Free(Void* pBlock) = 0;
};

class GraphicsManager
{
private:
    GraphicsManager(const GraphicsManager&) = delete;
    GraphicsManager& operator=(const GraphicsManager&) = delete;

    GraphicsManager(GraphicsManager&&) = delete;
    GraphicsManager& operator=(GraphicsManager&&) = delete;

public:
    static Bool Is();
    static GraphicsManager* Get();

    GraphicsManager(IAllocator* pAllocator);
    ~GraphicsManager();

    Void* MallocDebug(const Char* filename, UInt64 line, SizeT bytes, SizeT alignment);
    Void* Malloc(SizeT bytes, SizeT alignment);

    Void FreeDebug(const Char* filename, UInt64 line, Void* pBlock);
    Void Free(Void* pBlock);

private:
    static GraphicsManager* s_pInstance;
    IAllocator* m_pAllocator;
};

template<SizeT AlignmentSize = alignof(UInt64)>
class BaseAllocator
{
protected:
    BaseAllocator() = default;

    virtual ~BaseAllocator() = default;

public:
    Void* operator new(SizeT bytes, const Char* filename, UInt64 line)
    {
        if (!GraphicsManager::Is()) { return nullptr; }

        return GraphicsManager::Get()->MallocDebug(filename, line, bytes, AlignmentSize);
    }

    Void* operator new(SizeT bytes)
    {
        if (!GraphicsManager::Is()) { return nullptr; }

        return GraphicsManager::Get()->Malloc(bytes, AlignmentSize);
    }

    Void* operator new[](SizeT bytes, const Char* filename, UInt64 line)
    {
        return BaseAllocator::operator new(bytes, filename, line);
    }

    Void* operator new[](SizeT bytes)
    {
        return BaseAllocator::operator new(bytes);
    }

    Void operator delete(Void* pBlock)
    {
        if (!GraphicsManager::Is()) { return; }

        GraphicsManager::Get()->Free(pBlock);
    }

    Void operator delete(Void* pBlock, const Char* filename, Int32 line)
    {
        if (!GraphicsManager::Is()) { return; }

        GraphicsManager::Get()->FreeDebug(filename, line, pBlock);
    }

    Void operator delete[](Void* pBlock)
    {
        BaseAllocator::operator delete(pBlock);
    }

    Void operator delete[](Void* pBlock, const Char* file, Int32 line)
    {
        BaseAllocator::operator delete(pBlock, file, line);
    }
};

enum RESOURCE_FORMAT
{
    RESOURCE_FORMAT_NUM,
};

struct DeviceDesc
{
    Bool enableDebug;
};

struct SwapChainDesc
{
    RESOURCE_FORMAT resourceFormat;
    Void*           pWindowHandle;
};



class ISwapChain
{
protected:
    ISwapChain() = default;

    virtual ~ISwapChain() = default;

public:
    virtual SwapChainDesc GetDesc() const = 0;

    virtual Void Present() = 0;
};


class IDevice
{
protected:
    IDevice() = default;

    virtual ~IDevice() = default;

public:
    virtual DeviceDesc GetDesc() const = 0;

    virtual Bool CreateSwapChain(const SwapChainDesc& swapChainDesc, ISwapChain*& pOutSwapChain) = 0;
};

Bool CreateDevice(const DeviceDesc& deviceDesc, IDevice*& pOutDevice);


} // namespace Graphics
} // namespace Oak

