
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Core/Assert.hpp"

namespace Oak
{
namespace Random
{

class XorShift128
{
private:
    struct Dictionary
    {
        UInt32 x;
        UInt32 y;
        UInt32 z;
        UInt32 w;
    };
    UInt32 x;
    UInt32 y;
    UInt32 z;
    UInt32 w;

public:
    static const struct Dictionary defaults;
    UInt32 randCount = 0;
    struct Dictionary seeds;

    inline XorShift128(UInt32 w, UInt32 x = 0, UInt32 y = 0, UInt32 z = 0)
    {
        OAK_ASSERT(w != 0 || x != 0 || y != 0 || z != 0);

        x = x != 0 ? x : w << 13;
        y = y != 0 ? y : (w >> 9) ^ (x << 6);
        z = z != 0 ? z : y >> 7;
        seeds = {x, y, z, w};
        this->w = w;
        this->x = x;
        this->y = y;
        this->z = z;
    }

    inline UInt32 rand()
    {
        randCount++;
        UInt32 t = x ^ (x << 11);
        x = y;
        y = z;
        z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }

    inline Int32 randInt(Int32 min = 0, Int32 max = 0x7FFFFFFF)
    {
        return rand() % (max - min + 1) + min;
    }

    inline Float randFloat(Float min = 0.0f, Float max = 1.0f)
    {
        return (Float)(rand() % 0xFFFF) / 0xFFFF * (max - min) + min;
    }

    template <typename T>
    inline T* shuffle(const T* _arr, T* arr, Int32 arrLength)
    {
        for (Int32 i = 0; i < arrLength; i++)
            arr[i] = _arr[i];
        for (Int32 i = 0; i <= arrLength - 2; i++)
        {
            Int32 r = randInt(i, arrLength - 1);
            T tmp = arr[i];
            arr[i] = arr[r];
            arr[r] = tmp;
        }
        return arr;
    }

    static inline XorShift128 defaultSeed()
    {
        return XorShift128(XorShift128::defaults.w, XorShift128::defaults.x,
                           XorShift128::defaults.y, XorShift128::defaults.z);
    }
};

const struct XorShift128::Dictionary XorShift128::defaults = {
  123456789, 362436069, 521288629, 88675123};

} // namespace Random
} // namespace Oak
