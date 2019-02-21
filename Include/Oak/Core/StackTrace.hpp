
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{

class StackTrace
{
public:
    struct TraceInfo
    {
        Char function[256];
        Char file[256];
        Char moduleName[256];
        Int32 line;
        Void* lineAddress;
        Void* address;

        TraceInfo();

        Void Clear();

        Void Print();
    };

    static Void Initialize();

    static Void Terminate();

    static UInt64 CaptureStackTraceHash();

    static UInt32 CaptureStackTrace(TraceInfo* infoBuffer, UInt32 bufferCount);
};

} // namespace Oak
