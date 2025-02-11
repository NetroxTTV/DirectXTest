#pragma once

#include "D3DApp.h"
#include <DirectXMath.h>

class TriangleRenderer
{
public:
    void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
    void Render(ID3D12GraphicsCommandList* commandList);

private:
    void CreateVertexBuffer(ID3D12Device* device);
    void CreatePipelineStateObject(ID3D12Device* device);

    ID3D12Resource* mVertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
    ID3D12PipelineState* mPipelineState;
    ID3D12RootSignature* mRootSignature;
};
