#include "Oak/Platform/OS/Win32.hpp"

#include "Thread/ThreadTest.hpp"
#include "Thread/MutexTest.hpp"
#include "Thread/CriticalSectionTest.hpp"
#include "Thread/SemaphoreTest.hpp"
#include "Thread/EventTest.hpp"
#include "Timer/TimerTest.hpp"
#include "FileSystem/FileSystemTest.hpp"
#include "NetWork/NetWorkTest.hpp"


#define ExecuteTest(testName) testName ## TestMain()


int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    //ExecuteTest(Thread);
    //ExecuteTest(Mutex);
    //ExecuteTest(CriticalSection);
    //ExecuteTest(Semaphore);
    //ExecuteTest(Event);
    //ExecuteTest(Timer);
    //ExecuteTest(FileSystem);
    //ExecuteTest(NetWork);

    return 0;
}

