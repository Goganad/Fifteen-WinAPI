#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

#include "Game.h"

const COLORREF BACKGROUND_COLOR = RGB(255, 255, 255);
const SIZE FIRST_WINDOW_SIZE = SIZE{800, 600};
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

using namespace Gdiplus;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow){
    const wchar_t WINDOW_CLASS[] = L"MAIN_WINDOW_CLASS";
    const wchar_t WINDOW_TITLE[] = L"Fifteen++";

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    WNDCLASSEXW wc;
    wc.cbSize = sizeof(wc);
    wc.style = 0;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(BACKGROUND_COLOR);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = WINDOW_CLASS;
    wc.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);

    RegisterClassExW(&wc);

    HWND hwnd = CreateWindowExW(
            0x0,
            WINDOW_CLASS,
            WINDOW_TITLE,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, FIRST_WINDOW_SIZE.cx, FIRST_WINDOW_SIZE.cy,
            nullptr,
            nullptr,
            hInstance,
            nullptr
    );

    if (hwnd == nullptr) {
        MessageBoxW(nullptr, L"Error creating window", L"Attention", MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    GdiplusShutdown(gdiplusToken);

    return 0;
}