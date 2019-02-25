
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#if OAK_USE_HEAP_TRACKING

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{

class IPolicyWrapper
{
public:
    virtual const SizeT GetHash() const = 0;

    virtual Void* AllocateBytes(SizeT bytes) = 0;

    virtual Void* AllocateBytesAligned(SizeT bytes, SizeT alignment) = 0;

    virtual Void DeallocateBytes(Void* pBlock) = 0;

    virtual Void DeallocateBytesAligned(Void* pBlock, SizeT alignment) = 0;

    virtual Void* AllocateBytesForTracking(SizeT bytes) = 0;

    virtual Void* AllocateBytesAlignedForTracking(SizeT bytes,
                                                  SizeT alignment) = 0;

    virtual Void DeallocateBytesForTracking(Void* pBlock) = 0;

    virtual Void DeallocateBytesAlignedForTracking(Void* pBlock,
                                                   SizeT alignment) = 0;
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
class PolicyWrapper : public IPolicyWrapper
{
public:
    typedef Policy PolicyType;

    static inline PolicyWrapper* Get()
    {
        static PolicyWrapper instance;
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

    inline Void* AllocateBytesForTracking(SizeT bytes) override
    {
        return Policy::AllocateBytesForTracking(bytes);
    }

    inline Void* AllocateBytesAlignedForTracking(SizeT bytes, SizeT alignment)
      override
    {
        return Policy::AllocateBytesAlignedForTracking(bytes, alignment);
    }

    inline Void DeallocateBytesForTracking(Void* pBlock) override
    {
        Policy::DeallocateBytesForTracking(pBlock);
    }

    inline Void DeallocateBytesAlignedForTracking(Void* pBlock, SizeT alignment)
      override
    {
        Policy::DeallocateBytesAlignedForTracking(pBlock, alignment);
    }
};

} // namespace Oak

#endif // OAK_USE_HEAP_TRACKING
