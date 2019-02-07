
#include "CriticalSectionTest.hpp"


#include "Oak/Platform/Thread/Thread.hpp"
#include "Oak/Platform/Thread/CriticalSection.hpp"
#include <cstdio>


using namespace Oak;


namespace CriticalSectionTest {


Void ShowMessage(const Char* message)
{
    MessageBoxA(NULL, message, NULL, MB_OK);
}

//ウィンドウハンドル
HWND g_hwnd;
//インスタンスハンドル
HINSTANCE hinst;

//ウィンドウ横幅
const int WIDTH = 500;
const int HEIGHT = 300;

Oak::Thread* g_pThread1 = nullptr;
Oak::Thread* g_pThread2 = nullptr;
Oak::Bool g_exitThread = false;
Oak::CriticalSection* g_pCriticalSection = nullptr;
int g_count = 0;


UInt32 ThreadProc1(Void* pArgumentBlock, SizeT argumentSize)
{
    g_pCriticalSection->Lock();

    UNREFERENCED_PARAMETER(pArgumentBlock);
    UNREFERENCED_PARAMETER(argumentSize);

    char buf[1000];

    while (!g_exitThread && g_count <= 1000)
    {
        sprintf_s(buf, "カウント数表示 : %d", g_count);
        SetWindowTextA(g_hwnd, buf);

        ++g_count;

        Thread::Sleep(10);
    }

    g_pCriticalSection->Unlock();

    return 0;
}

UInt32 ThreadProc2(Void* pArgumentBlock, SizeT argumentSize)
{
    g_pCriticalSection->Lock();

    UNREFERENCED_PARAMETER(pArgumentBlock);
    UNREFERENCED_PARAMETER(argumentSize);

    char buf[1000];

    while (!g_exitThread && g_count >= 0)
    {
        sprintf_s(buf, "カウント数表示 : %d", g_count);
        SetWindowTextA(g_hwnd, buf);

        --g_count;

        Thread::Sleep(10);
    }

    g_pCriticalSection->Unlock();

    return 0;
}


LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CREATE:
        //スレッドを作成
        g_pThread1 = new Oak::Thread("スレッド1", ThreadProc1);
        g_pThread2 = new Oak::Thread("スレッド2", ThreadProc2);
        g_pCriticalSection = new Oak::CriticalSection();
        g_pThread1->Start(nullptr, 0);
        g_pThread2->Start(nullptr, 0);
        return 0;
    case WM_CLOSE:
        g_exitThread = true;
        g_pThread1->Wait();
        g_pThread2->Wait();
        delete g_pThread1;
        delete g_pThread2;
        delete g_pCriticalSection;
        //ウィンドウを破棄
        DestroyWindow(hwnd);
        return 0;

    }
    return DefWindowProcA(hwnd, msg, wp, lp);
}


} // namespace CriticalSectionTest


int CriticalSectionTestMain()
{
    using namespace CriticalSectionTest;

    HINSTANCE hInstance = GetModuleHandleA(NULL);
    MSG msg;
    WNDCLASSA wc;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WinProc;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hCursor = wc.hIcon = NULL;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = "test";
    wc.lpszMenuName = NULL;

    if (!RegisterClassA(&wc)) {
        ShowMessage("クラスの登録失敗");
        return -1;
    }

    g_hwnd = CreateWindowA("test", "テストウィンドウ", WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        0, 0, 400, 400, NULL, NULL, hInstance, NULL);

    if (g_hwnd == NULL) {
        ShowMessage("ウィンドウ作成失敗");
        return -1;
    }

    //インスタンスハンドル
    hinst = hInstance;

    //エラーチェック用変数
    int check;

    check = GetMessageA(&msg, NULL, 0, 0);
    while (check) {
        check = GetMessageA(&msg, NULL, 0, 0);
        if (check == -1) {
            break;
        }
        DispatchMessageA(&msg);
    }

    //クラス解放
    UnregisterClassA("test", hinst);

    return 0;
}

