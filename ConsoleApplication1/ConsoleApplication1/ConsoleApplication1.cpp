#include <winerror.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <d3d12sdklayers.h>
#include <wrl/client.h>
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <tchar.h>
#include <windows.h>
#include "d3dx12.h"
#include "Utils.h"
#include "D3DApp.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#define ThrowIfFailed(hr) if (FAILED(hr)) throw std::runtime_error("HRESULT failed at " + std::string(__FILE__) + ":" + std::to_string(__LINE__))

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    D3DApp app;
    try
    {
#if defined(DEBUG) || defined(_DEBUG) 
        {
            ID3D12Debug* debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
                debugController->EnableDebugLayer();
            }
        }
#endif

    app.InitWindow(hInstance, nCmdShow);
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&app.mdxgiFactory)));

    HRESULT hardwareResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&app.md3dDevice));
    if (FAILED(hardwareResult)) {
        IDXGIAdapter* pWarpAdapter;
        ThrowIfFailed(app.mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
        ThrowIfFailed(D3D12CreateDevice(pWarpAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&app.md3dDevice)));
    }

    app.InitializeResources();
    app.CreateCommandObjects();
    app.CreateRtvAndDsvDescriptorHeaps();
    app.CreateSwapChain();
    app.CreateRenderTargetViews();

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            app.Render();
        }
    }
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}