
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{

class Log
{
public:
    enum Level
    {
        Verbose,
        Debug,
        Info,
        Warning,
        Error,
        Assert,
        Num
    };

    static Void Format(Level level, const Char* format, ...);
    static Void Message(Level level, const Char* message);

    static Void Format(const Char* format, ...);
    static Void Message(const Char* message);
};

} // namespace Oak
