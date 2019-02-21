
#pragma once

#include "Oak/Platform/PlatformDetection.hpp"

#if defined(OAK_PLATFORM_WIN)

#include "Oak/Platform/Thread/Win32/XEvent_Win32.hpp"

#else

#error "this platform is not supported."

#endif