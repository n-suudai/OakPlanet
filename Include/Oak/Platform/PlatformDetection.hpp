
#pragma once


#if defined(_WIN32) || defined(WIN32)

#include "Oak/Platform/OS/Win32.hpp"

#else

#error "this platform is not supported."

#endif


#ifndef DECL_MALLOC

#if defined(_MSC_VER)

#define DECL_MALLOC __declspec(restrict) __declspec(noalias)

#else // !defined(_MSC_VER)

#define DECL_MALLOC __attribute__ ((malloc))

#endif

#endif // DECL_MALLOC

