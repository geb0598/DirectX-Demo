#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#include <wrl.h>

namespace DXD
{

	class URenderer
	{
	public:
		static URenderer& GetInstance(HWND HWindow);

		~URenderer();

		URenderer(const URenderer&) = delete;
		URenderer(URenderer&&) = delete;
		URenderer& operator=(const URenderer&) = delete;
		URenderer& operator=(URenderer&&) = delete;

		void Prepare();
		void Render();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

	private:
		URenderer(HWND HWindow);

		void Create(HWND HWindow);
		void CreateDeviceAndSwapChain(HWND HWindow);
		void CreateFrameBuffer();
		void CreateRasterizerState();

		Microsoft::WRL::ComPtr<ID3D11Device> Device						  = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext		  = nullptr;
		Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain				  = nullptr;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> FrameBuffer				  = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> FrameBufferRTV	  = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState	  = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer				  = nullptr;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilBuffer		  = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView   = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState = nullptr;

		FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };
		D3D11_VIEWPORT ViewportInfo;
	};

} // namespace DXD
