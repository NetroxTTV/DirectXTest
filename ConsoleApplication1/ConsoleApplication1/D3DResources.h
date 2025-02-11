#pragma once
#include <d3d12.h>
#include <wrl/client.h>

class D3DResources {
public:
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors);
};
