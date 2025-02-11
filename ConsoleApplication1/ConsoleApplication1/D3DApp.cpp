#include "D3DApp.h"

void D3DApp::CreateRtvAndDsvDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;
    ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap)));

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDesc.NodeMask = 0;
    ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDsvHeap)));
}

void D3DApp::CreateSwapChain()
{
    // Lib�rer la swap chain existante
    if (mSwapChain)
    {
        mSwapChain->Release();
        mSwapChain = nullptr;
    }

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width = mClientWidth;
    swapChainDesc.BufferDesc.Height = mClientHeight;
    swapChainDesc.BufferDesc.Format = mBackBufferFormat;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = mhMainWnd;
    swapChainDesc.SampleDesc.Count = 1; // No MSAA for now
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.BufferCount = SwapChainBufferCount;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    IDXGISwapChain* tempSwapChain;
    ThrowIfFailed(mdxgiFactory->CreateSwapChain(
        mCommandQueue,
        &swapChainDesc,
        &tempSwapChain
    ));

    ThrowIfFailed(tempSwapChain->QueryInterface(IID_PPV_ARGS(&mSwapChain)));
    tempSwapChain->Release();

    mCurrBackBuffer = 0;
}

void D3DApp::InitializeResources()
{
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels = {};
    msQualityLevels.Format = mBackBufferFormat;
    msQualityLevels.SampleCount = 4;
    msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    msQualityLevels.NumQualityLevels = 0;

    ThrowIfFailed(md3dDevice->CheckFeatureSupport(
        D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
        &msQualityLevels,
        sizeof(msQualityLevels)));

    m4xMsaaQuality = msQualityLevels.NumQualityLevels;
    assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

    ThrowIfFailed(md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));

    mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    //gTriangleRenderer.Initialize(md3dDevice, mCommandList);
}

void D3DApp::CreateCommandObjects()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    ThrowIfFailed(md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));
    ThrowIfFailed(md3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mDirectCmdListAlloc)));
    ThrowIfFailed(md3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mDirectCmdListAlloc, nullptr, IID_PPV_ARGS(&mCommandList)));
    mCommandList->Close();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void D3DApp::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    mWindow.InitWindow(hInstance, nCmdShow);
    mhMainWnd = mWindow.GetMainWnd();
}

void D3DApp::CreateRenderTargetViews()
{
    if (mSwapChain == nullptr)
    {
        std::cerr << "Swap chain is not initialized." << std::endl;
        return;
    }

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());

    for (UINT i = 0; i < SwapChainBufferCount; i++)
    {
        ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mRenderTargets[i])));
        md3dDevice->CreateRenderTargetView(mRenderTargets[i], nullptr, rtvHandle);
        rtvHandle.Offset(1, mRtvDescriptorSize);
    }
}

void D3DApp::Render()
{
    if (mRenderTargets[mCurrBackBuffer] == nullptr)
    {
        std::cerr << "Render target is not initialized." << std::endl;
        return;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
    rtvHandle.ptr += mCurrBackBuffer * mRtvDescriptorSize;

    const float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };

    // Reset the command list before recording commands
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc, nullptr));

    auto transitionToRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
        mRenderTargets[mCurrBackBuffer],
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    mCommandList->ResourceBarrier(1, &transitionToRenderTarget);

    mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // Dessiner le triangle
    //gTriangleRenderer.Render(mCommandList);

    auto transitionToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
        mRenderTargets[mCurrBackBuffer],
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);

    mCommandList->ResourceBarrier(1, &transitionToPresent);

    // Close the command list
    ThrowIfFailed(mCommandList->Close());

    ID3D12CommandList* cmdsLists[] = { mCommandList };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Present the frame
    ThrowIfFailed(mSwapChain->Present(1, 0));
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;
}