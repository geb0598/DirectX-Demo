#include "dxd/renderer.h"

namespace dxd
{

	URenderer& URenderer::GetInstance(HWND HWindow)
	{
		// NOTE: Renderer is initialized once per execution as a singleton
		static URenderer Renderer(HWindow);
		return Renderer;
	}

	URenderer::~URenderer()
	{
		if (DeviceContext)
		{
			DeviceContext->Flush();
		}
	}

	void URenderer::Prepare()
	{
		DeviceContext->ClearRenderTargetView(FrameBufferRTV.Get(), ClearColor);

		DeviceContext->RSSetViewports(1, &ViewportInfo);
		DeviceContext->RSSetState(RasterizerState.Get());

		DeviceContext->OMSetRenderTargets(1, FrameBufferRTV.GetAddressOf(), nullptr);
		DeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	}

	void URenderer::Render()
	{
		SwapChain->Present(1, 0);
	}

	ID3D11Device* URenderer::GetDevice()
	{
		return Device.Get();
	}

	ID3D11DeviceContext* URenderer::GetDeviceContext()
	{
		return DeviceContext.Get();
	}

	URenderer::URenderer(HWND HWindow)
	{
		Create(HWindow);
	}

	void URenderer::Create(HWND HWindow)
	{
		CreateDeviceAndSwapChain(HWindow);

		CreateFrameBuffer();

		CreateRasterizerState();
	}

	void URenderer::CreateDeviceAndSwapChain(HWND HWindow)
	{
		D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc.Width = 0;
		SwapChainDesc.BufferDesc.Height = 0;
		SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.OutputWindow = HWindow;
		SwapChainDesc.Windowed = TRUE;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
			FeatureLevels, ARRAYSIZE(FeatureLevels), D3D11_SDK_VERSION,
			&SwapChainDesc, &SwapChain, &Device, nullptr, &DeviceContext);

		SwapChain->GetDesc(&SwapChainDesc);

		ViewportInfo
			= { 0.0f, 0.0f, (float)SwapChainDesc.BufferDesc.Width, (float)SwapChainDesc.BufferDesc.Height, 0.0f, 1.0f };
	}

	void URenderer::CreateFrameBuffer()
	{
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)FrameBuffer.GetAddressOf());

		D3D11_RENDER_TARGET_VIEW_DESC FrameBufferRTVDesc = {};
		FrameBufferRTVDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		FrameBufferRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		Device->CreateRenderTargetView(FrameBuffer.Get(), &FrameBufferRTVDesc, FrameBufferRTV.GetAddressOf());
	}

	void URenderer::CreateRasterizerState()
	{
		D3D11_RASTERIZER_DESC RasterizerDesc = {};
		RasterizerDesc.FillMode = D3D11_FILL_SOLID;
		RasterizerDesc.CullMode = D3D11_CULL_BACK;

		Device->CreateRasterizerState(&RasterizerDesc, RasterizerState.GetAddressOf());
	}

} // namespace dxdj