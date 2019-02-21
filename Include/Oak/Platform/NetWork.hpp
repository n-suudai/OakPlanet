
#pragma once

#include "Oak/Platform/PlatformDetection.hpp"

#if defined(OAK_PLATFORM_WIN)

#include "Oak/Platform/NetWork/Win32/XSocket_Win32.hpp"

#else

#error "this platform is not supported."

#endif
