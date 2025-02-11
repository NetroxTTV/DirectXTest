#include "Window.h"
#include <tchar.h>

void Window::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("D3D12WindowClass"), NULL };
    RegisterClassEx(&wc);
    mhMainWnd = CreateWindow(wc.lpszClassName, _T("DirectX 12"), WS_OVERLAPPEDWINDOW, 100, 100, mClientWidth, mClientHeight, NULL, NULL, wc.hInstance, NULL);
    ShowWindow(mhMainWnd, SW_SHOW);
    UpdateWindow(mhMainWnd);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
