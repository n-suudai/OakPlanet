#include "Oak/Platform/OS/Win32.hpp"

#include "Thread/ThreadTest.hpp"
#include "Thread/MutexTest.hpp"
#include "Thread/CriticalSectionTest.hpp"
#include "Thread/SemaphoreTest.hpp"
#include "Thread/EventTest.hpp"

#include  <chrono>


#define ExecuteTest(testName) testName ## TestMain()


int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    //ExecuteTest(Thread);
    //ExecuteTest(Mutex);
    //ExecuteTest(CriticalSection);
    //ExecuteTest(Semaphore);
    ExecuteTest(Event);

    return EXIT_SUCCESS;
}

