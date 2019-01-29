
#pragma once


#include "Oak/Core/Types.hpp"


namespace Oak {
namespace Core {


class Log
{
public:
    enum Level
    {
        Verbose
        , Debug
        , Info
        , Warning
        , Error
        , Assert
        , Num
    };

    static Void Format(Level level, const Char* format, ...);
    static Void Message(Level level, const Char* message);

    static Void Format(const Char* format, ...);
    static Void Message(const Char* message);
};


} // namespace Core
} // namespace Oak

