
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Core/DebugBreak.hpp"

#ifndef OAK_ASSERT
#ifdef _DEBUG
#define OAK_ASSERT(expression)                               \
    static_cast<Oak::Void>(                                  \
      (!!(expression)) ||                                    \
      (Oak::Assert::Handle(#expression, __FILE__, __LINE__), \
       OAK_DEBUG_BREAK(), false))

#else
#define OAK_ASSERT(expression) static_cast<Oak::Void>(0)
#endif
#endif // OAK_ASSERT_MESSAGE

#ifndef OAK_ASSERT_MESSAGE
#ifdef _DEBUG
#define OAK_ASSERT_MESSAGE(expression, message)                              \
    static_cast<Oak::Void>(                                                  \
      (!!(expression)) ||                                                    \
      (Oak::Assert::HandleMessage(#expression, message, __FILE__, __LINE__), \
       OAK_DEBUG_BREAK(), false))

#else
#define OAK_ASSERT_MESSAGE(expression, message) static_cast<Oak::Void>(0)
#endif
#endif // OAK_ASSERT_MESSAGE

namespace Oak
{

class Assert
{
public:
    static Void Handle(const Char* expression, const Char* file, Int32 line);

    static Void HandleMessage(const Char* expression, const Char* message,
                              const Char* file, Int32 line);
};

} // namespace Oak
