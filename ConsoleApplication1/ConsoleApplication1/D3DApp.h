#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <vector>
#include "Utils.h"
#include "d3dx12.h"
#include <tchar.h>
#include <iostream> 
#include <cassert>
#include "Window.h"
#include "TriangleRenderer.h"

class D3DApp {
public:
    void InitializeResources();
    void InitWindow(HINSTANCE hInstance, int nCmdShow);
    void Render();
    void CreateCommandObjects();
    void CreateSwapChain();
    void CreateRenderTargetViews();
    void CreateRtvAndDsvDescriptorHeaps();

    HINSTANCE mhAppInst = nullptr;
    HWND mhMainWnd = nullptr;
    std::wstring mMainWndCaption = L"D3D12 Application";

    Window mWindow;
    ID3D12Device* md3dDevice;
    ID3D12Fence* mFence;
    IDXGIFactory4* mdxgiFactory;
    ID3D12CommandQueue* mCommandQueue;
    ID3D12CommandAllocator* mDirectCmdListAlloc;
    ID3D12GraphicsCommandList* mCommandList;
private:
    IDXGISwapChain3* mSwapChain;
    ID3D12DescriptorHeap* mRtvHeap;
    ID3D12DescriptorHeap* mDsvHeap;
    ID3D12Resource* mRenderTargets[2];

    UINT mClientWidth = 800;
    UINT mClientHeight = 600;
    UINT SwapChainBufferCount = 2;
    UINT mRtvDescriptorSize = 0;
    UINT mDsvDescriptorSize = 0;
    UINT mCbvSrvDescriptorSize = 0;
    UINT mCurrBackBuffer = 0;
    UINT m4xMsaaQuality = 0;
    bool m4xMsaaState = true;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
};
