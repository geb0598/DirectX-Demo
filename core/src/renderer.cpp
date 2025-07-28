#include <d3d11.h>
#include <d3dcompiler.h>

#include <wrl.h>

#include "dxd/renderer.h"
#include "dxd/log.h"

namespace DXD
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
		DeviceContext->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		DeviceContext->RSSetViewports(1, &ViewportInfo);
		DeviceContext->RSSetState(RasterizerState.Get());

		// DeviceContext->OMSetRenderTargets(1, FrameBufferRTV.GetAddressOf(), nullptr);
		DeviceContext->OMSetRenderTargets(1, FrameBufferRTV.GetAddressOf(), DepthStencilView.Get());
		DeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
		DeviceContext->OMSetDepthStencilState(DepthStencilState.Get(), 1);
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
		D3D_FEATURE_LEVEL FeatureLevels[]  = { D3D_FEATURE_LEVEL_11_0 };
		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc.Width	   = 0;
		SwapChainDesc.BufferDesc.Height    = 0;
		SwapChainDesc.BufferDesc.Format	   = DXGI_FORMAT_B8G8R8A8_UNORM;
		SwapChainDesc.SampleDesc.Count	   = 1;
		SwapChainDesc.SampleDesc.Quality   = 0;
		SwapChainDesc.BufferUsage		   = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount		   = 2;
		SwapChainDesc.OutputWindow		   = HWindow;
		SwapChainDesc.Windowed			   = TRUE;
		SwapChainDesc.SwapEffect		   = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
			FeatureLevels, ARRAYSIZE(FeatureLevels), D3D11_SDK_VERSION,
			&SwapChainDesc, &SwapChain, &Device, nullptr, &DeviceContext);

		SwapChain->GetDesc(&SwapChainDesc);

		ViewportInfo.TopLeftX = 0.0f;
		ViewportInfo.TopLeftY = 0.0f;
		ViewportInfo.Width	  = (float)SwapChainDesc.BufferDesc.Width;
		ViewportInfo.Height   = (float)SwapChainDesc.BufferDesc.Height;
		ViewportInfo.MinDepth = 0.0f;
		ViewportInfo.MaxDepth = 1.0f;

		//------------------ Create Depth Stencil Buffer ------------------//
		D3D11_TEXTURE2D_DESC DepthStencilDesc = {};
		DepthStencilDesc.Width				  = SwapChainDesc.BufferDesc.Width;
		DepthStencilDesc.Height				  = SwapChainDesc.BufferDesc.Height;
		DepthStencilDesc.MipLevels			  = 1;
		DepthStencilDesc.ArraySize			  = 1;
		DepthStencilDesc.Format				  = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilDesc.SampleDesc.Count	  = 1;
		DepthStencilDesc.SampleDesc.Quality	  = 0;
		DepthStencilDesc.Usage				  = D3D11_USAGE_DEFAULT;
		DepthStencilDesc.BindFlags			  = D3D11_BIND_DEPTH_STENCIL;
		DepthStencilDesc.CPUAccessFlags		  = 0;
		DepthStencilDesc.MiscFlags			  = 0;

		Device->CreateTexture2D(&DepthStencilDesc, nullptr, DepthStencilBuffer.GetAddressOf());

		Device->CreateDepthStencilView(DepthStencilBuffer.Get(), nullptr, DepthStencilView.GetAddressOf());

		D3D11_DEPTH_STENCIL_DESC DepthStencilStateDesc = {};
		DepthStencilStateDesc.DepthEnable    = TRUE;
		DepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthStencilStateDesc.DepthFunc		 = D3D11_COMPARISON_LESS;
		DepthStencilStateDesc.StencilEnable  = FALSE;

		Device->CreateDepthStencilState(&DepthStencilStateDesc, DepthStencilState.GetAddressOf());
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

} // namespace DXD