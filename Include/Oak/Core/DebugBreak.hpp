
#pragma once

#if defined(_MSC_VER)
#include <xutility>
#endif

#ifndef OAK_DEBUG_BREAK
#if defined(_MSC_VER)
#define OAK_DEBUG_BREAK() _CrtDbgBreak()
#else
#define OAK_DEBUG_BREAK() asm("int $3")
#endif
#endif
