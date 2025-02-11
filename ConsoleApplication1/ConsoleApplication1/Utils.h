#pragma once
#include <stdexcept>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "d3dx12.h"
#include <wrl/client.h>

// Macro to check HRESULT and throw exception on failure
#define ThrowIfFailed(hr) if (FAILED(hr)) throw std::runtime_error("HRESULT failed at " + std::string(__FILE__) + ":" + std::to_string(__LINE__))

// Utility class for descriptor handles
class DescriptorHandle {
public:
    DescriptorHandle() : ptr(0) {}
    explicit DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle) : ptr(handle.ptr) {}

    void Offset(UINT size) { ptr += size; }
    D3D12_CPU_DESCRIPTOR_HANDLE Get() const { return { ptr }; }

private:
    SIZE_T ptr;
};
