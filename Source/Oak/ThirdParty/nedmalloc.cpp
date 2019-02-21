

#ifdef OAK_USE_NEDMALLOC

#include "Oak/ThirdParty/nedmalloc.hpp"

#pragma warning(push)
#pragma warning(disable : 4189)
#pragma warning(disable : 4505)
#pragma warning(disable : 4702)
#include "nedmalloc/nedmalloc.c"
#pragma warning(pop)

#endif // OAK_USE_NEDMALLOC
