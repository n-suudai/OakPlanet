
#include "TimerTest.hpp"

#include "Oak/Platform/Timer.hpp"
#include "Oak/Platform/Thread/Thread.hpp"
#include <cstdio>


int TimerTestMain()
{
    {
        Oak::HighResolutionTimer timer;

        Oak::Double start = timer.NowSeconds();

        Oak::Thread::Sleep(1000);

        Oak::Double elapsed = timer.NowSeconds() - start;

        char buff[256];
        sprintf_s(buff, "HighResolutionTimer : %lf (sec).\n", elapsed);

        OutputDebugStringA(buff);
    }

    {
        Oak::Double start = Oak::StandardTimer::NowSeconds();

        Oak::Thread::Sleep(1000);

        Oak::Double elapsed = Oak::StandardTimer::NowSeconds() - start;

        char buff[256];
        sprintf_s(buff, "StandardTimer : %lf (sec).\n", elapsed);

        OutputDebugStringA(buff);
    }

    return 0;
}

