#include "Oak/Platform/OS/Win32.hpp"

#include "Thread/ThreadTest.hpp"
#include "Thread/MutexTest.hpp"
#include "Thread/CriticalSectionTest.hpp"
#include "Thread/SemaphoreTest.hpp"
#include "Thread/EventTest.hpp"


#define ExecuteTest(testName) testName ## TestMain()


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    //ExecuteTest(Thread);
    //ExecuteTest(Mutex);
    //ExecuteTest(CriticalSection);
    //ExecuteTest(Semaphore);
    ExecuteTest(Event);

}

