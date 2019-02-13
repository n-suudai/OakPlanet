#include "AssertTest.hpp"
#include "Oak/Core/Assert.hpp"


int AssertTestMain()
{
    OAK_ASSERT(false);

    OAK_ASSERT_MESSAGE(false, "message");

    return 0;
}

