﻿
#include "ThreadTest.hpp"


#include "Oak/Platform/Thread/Thread.hpp"
#include "Oak/Platform/Thread/Semaphore.hpp"
#include <cstdio>


using namespace Oak;


namespace SemaphoreTest {


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

Oak::Thread* g_pThread = nullptr;
Oak::Semaphore* g_pSemaphore = nullptr;
int g_count = 0;


UInt32 ThreadProc(Void* pArgumentBlock, SizeT argumentSize)
{
    g_pSemaphore->Lock();

    UNREFERENCED_PARAMETER(pArgumentBlock);
    UNREFERENCED_PARAMETER(argumentSize);

    char buf[1000];

    while (g_count < 1000)
    {
        sprintf_s(buf, "カウント数表示 : %d", g_count);
        SetWindowTextA(g_hwnd, buf);

        ++g_count;

        Thread::Sleep(1000);
    }

    g_pSemaphore->Unlock();

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
        g_pThread = new Oak::Thread("スレッド", ThreadProc);
        g_pSemaphore = new Oak::Semaphore("SEMAPHORE", 2, 2);
        g_pThread->Start(nullptr, 0);
        return 0;

    case WM_CLOSE:
        //フラグをfalseにしてスレッドを終了させる。
        g_pThread->Wait();
        delete g_pThread;
        delete g_pSemaphore;
        //ウィンドウを破棄
        DestroyWindow(hwnd);
        return 0;

    }
    return DefWindowProcA(hwnd, msg, wp, lp);
}


} // namespace SemaphoreTest


int SemaphoreTestMain()
{
    using namespace SemaphoreTest;

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

