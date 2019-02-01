
#pragma once


#ifndef STRICT

#define STRICT

#endif


#ifndef WIN32_LEAN_AND_MEAN

#define WIN32_LEAN_AND_MEAN

#endif


#if !defined(NOMINMAX) && defined(_MSC_VER)

#define NOMINMAX

#endif


#include <Windows.h>
#include <tchar.h>


#ifndef OAK_PLATFORM_WIN

#define OAK_PLATFORM_WIN

#endif

