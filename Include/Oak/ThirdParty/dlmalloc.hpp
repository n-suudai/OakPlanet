
#pragma once


#ifdef OAK_USE_DLMALLOC


#ifndef USE_DL_PREFIX
#define USE_DL_PREFIX
#endif


#ifndef MSPACES
#define MSPACES 1
#endif


#ifndef NO_INCLUDE_DLMALLOC_H
#include "dlmalloc/malloc.h"
#endif


#endif // OAK_USE_DLMALLOC

