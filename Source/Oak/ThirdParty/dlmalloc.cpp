

#ifdef  OAK_USE_DLMALLOC


// for C2733
#define NO_INCLUDE_DLMALLOC_H
#include "Oak/ThirdParty/dlmalloc.hpp"
#undef NO_INCLUDE_DLMALLOC_H


#pragma warning(push)
#pragma warning(disable:4127)
#pragma warning(disable:4702)
#include "dlmalloc/malloc.c"
#pragma warning(pop)


#endif // OAK_USE_DLMALLOC

