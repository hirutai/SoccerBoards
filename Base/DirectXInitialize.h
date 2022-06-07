#pragma once  
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <cstdlib>
#include <wrl.h>

#include "WindowsAPI.h"

/// <summary>
/// DirectX�������N���X
/// </summary>
class DirectXInitialize
{
#pragma region �G�C���A�X
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="winApi">�E�B���h�E</param>
	void Initialize(WindowsApi* winApi);

	/// <summary>
	/// �`��O����
	/// </summary>
	void BeforeDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void AfterDraw();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�N���A
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// �[�x�o�b�t�@�N���A
	/// </summary>
	void ClearDepthBuffer();
#pragma endregion

#pragma region getter
public:
	/// <summary>
	/// �f�o�C�X�̎擾
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDevice() { return device.Get(); }

	/// <summary>
	/// �R�}���h���X�g�̎擾
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }
#pragma endregion

#pragma region �����o�֐�
private:
	// DXGI�f�o�C�X����
	bool CreateDXGIDevice();
	// �R�}���h�֘A����
	bool CreateCommand();
	// �X���b�v�`�F�[������
	bool CreateSwapChain();
	// �����_�[�^�[�Q�b�g����
	bool CreateRenderTargets();
	// �[�x�o�b�t�@����
	bool CreateDepthBuffer();
	// �t�F���X����
	bool CreateFence();
#pragma endregion

#pragma region �����o�ϐ�
private:
	WindowsApi* winApi = nullptr; // �E�B���h�E

	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<IDXGISwapChain4> swapchain;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	D3D12_RESOURCE_BARRIER barrierDesc{};
#pragma endregion
};