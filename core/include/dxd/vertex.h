#pragma once

#include <type_traits>
#include <vector>

#include <DirectXMath.h>

#include <d3d11.h>
#include <d3dcompiler.h>

namespace DXD
{
	// -------------------------------------------------------------------------------- //
	//								Vertex Data Structures							    //
	// -------------------------------------------------------------------------------- //
	struct FVERTEX_P
	{
		DirectX::XMFLOAT3 Position;

		static std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayoutDesc()
		{
			return
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA}
			};
		}
	};

	struct FVERTEX_PC
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;

		static std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayoutDesc()
		{
			return 
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA}
			};
		}
	};

	struct FVERTEX_PN
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;

		static std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayoutDesc()
		{
			return 
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA}
			};
		}
	};

	struct FVERTEX_PNT
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TexCoord;

		static std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayoutDesc()
		{
			return 
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA}
			};
		}
	};

	template<typename TVertex, typename = void>
	struct IsVertex : std::false_type {};

	template<typename TVertex>
	struct IsVertex<TVertex, std::void_t<decltype(TVertex::GetInputLayoutDesc)>> : std::true_type {};

} // namespace DXD
