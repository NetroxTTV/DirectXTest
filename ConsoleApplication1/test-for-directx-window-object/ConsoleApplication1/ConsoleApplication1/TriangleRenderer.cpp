//#include "TriangleRenderer.h"
//#include <d3dcompiler.h>
//
//struct Vertex
//{
//    DirectX::XMFLOAT3 Position;
//    DirectX::XMFLOAT4 Color;
//};
//
//Vertex vertices[] =
//{
//    { { 0.0f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
//    { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
//    { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
//};
//
//void TriangleRenderer::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
//{
//    CreateVertexBuffer(device);
//    CreatePipelineStateObject(device);
//}
//
//void TriangleRenderer::CreateVertexBuffer(ID3D12Device* device)
//{
//    const UINT vertexBufferSize = sizeof(vertices);
//
//    ThrowIfFailed(device->CreateCommittedResource(
//        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//        D3D12_HEAP_FLAG_NONE,
//        &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
//        D3D12_RESOURCE_STATE_GENERIC_READ,
//        nullptr,
//        IID_PPV_ARGS(&mVertexBuffer)));
//
//    UINT8* pVertexDataBegin;
//    CD3DX12_RANGE readRange(0, 0);
//    ThrowIfFailed(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
//    memcpy(pVertexDataBegin, vertices, sizeof(vertices));
//    mVertexBuffer->Unmap(0, nullptr);
//
//    mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
//    mVertexBufferView.StrideInBytes = sizeof(Vertex);
//    mVertexBufferView.SizeInBytes = vertexBufferSize;
//}
//
//void TriangleRenderer::CreatePipelineStateObject(ID3D12Device* device)
//{
//    ID3DBlob* vertexShader;
//    ID3DBlob* pixelShader;
//
//    UINT compileFlags = 0;
//    ThrowIfFailed(D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
//    ThrowIfFailed(D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));
//
//    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
//    {
//        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
//    };
//
//    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
//    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
//    psoDesc.pRootSignature = mRootSignature;
//    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader);
//    psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader);
//    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
//    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
//    psoDesc.DepthStencilState.DepthEnable = FALSE;
//    psoDesc.DepthStencilState.StencilEnable = FALSE;
//    psoDesc.SampleMask = UINT_MAX;
//    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//    psoDesc.NumRenderTargets = 1;
//    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
//    psoDesc.SampleDesc.Count = 1;
//
//    ThrowIfFailed(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipelineState)));
//}
//
//void TriangleRenderer::Render(ID3D12GraphicsCommandList* commandList)
//{
//    commandList->SetPipelineState(mPipelineState);
//    commandList->SetGraphicsRootSignature(mRootSignature);
//    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//    commandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
//    commandList->DrawInstanced(3, 1, 0, 0);
//}
