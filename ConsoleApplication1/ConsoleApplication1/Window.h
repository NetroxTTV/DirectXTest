#pragma once

#include <windows.h>

class Window
{
public:
    void InitWindow(HINSTANCE hInstance, int nCmdShow);

    HWND GetMainWnd() const { return mhMainWnd; }

private:
    HWND mhMainWnd = nullptr;
    int mClientWidth = 800;
    int mClientHeight = 600;
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
