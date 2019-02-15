
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"


namespace Oak {


class Bitwise
{
public:
    template<typename T>
    static inline Bool IsPowerOf2(T value)
    {
        return  !(value & (value - 1))
    }
};


}  // namespace Oak

