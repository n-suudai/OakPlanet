
#pragma once


#if defined(OAK_PLATFORM_WIN)

#include "Platform/Win32.hpp"

#elif defined(OAK_PLATFORM_OSX)

#include "Platform/OSX.hpp"

#else

#error platform is not defined.

#endif

