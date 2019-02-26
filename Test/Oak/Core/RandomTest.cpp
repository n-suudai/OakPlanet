
#include "RandomTest.hpp"
#include "Oak/Core/Random.hpp"
#include "Oak/Core/Log.hpp"

int RandomTestMain()
{
    using namespace Oak;

    Random::XorShift random(3);

    for (Int32 i = 0; i < 100; ++i)
    {
        Log::Format("Rand : %d\n", random.randInt(0, 100));

        Log::Format("Rand : %f\n", random.randFloat(0.0f, 5.0f));
    }

    return 0;
}
