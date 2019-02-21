
#pragma once

#include "Oak/Platform/PlatformDetection.hpp"

#if defined(OAK_PLATFORM_WIN)

#include "Oak/Platform/FileSystem/Win32/XFileSystem_Win32.hpp"

#else

#error "this platform is not supported."

#endif
