#pragma once

#include <type_traits>
#include <vector>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <wrl.h>

#include "vertex.h"

namespace DXD
{
	// -------------------------------------------------------------------------------- //
	//				           Interface and Helper functions       				    //
	// -------------------------------------------------------------------------------- //
	class IMesh
	{
	public:
		virtual ~IMesh() = default;
		
		virtual void Bind(ID3D11DeviceContext* DeviceContext) const = 0;

		virtual std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayoutDesc() const = 0;
		virtual UINT GetVertexCount() const = 0;
		virtual UINT GetIndexCount() const = 0;
		virtual UINT GetStride() const = 0;
	};

	// -------------------------------------------------------------------------------- //
	//							Implmentations and Details								//
	// -------------------------------------------------------------------------------- //
	template<typename TVertex, typename TEnable = std::enable_if_t<IsVertex<TVertex>::value>>
	class UMesh : public IMesh
	{
	public:
		virtual ~UMesh() = default;

		UMesh(ID3D11Device* Device, const std::vector<TVertex>& Vertices, const std::vector<UINT>& Indices);

		UMesh(const UMesh&) = delete;
		UMesh& operator=(const UMesh&) = delete;

		UMesh(UMesh&&) noexcept = default;
		UMesh& operator=(UMesh&&) noexcept = default;

		virtual void Bind(ID3D11DeviceContext* DeviceContext) const override;

		virtual std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayoutDesc() const override;
		virtual UINT GetVertexCount() const override;
		virtual UINT GetIndexCount() const override;
		virtual UINT GetStride() const override;

	private:
		void CreateMesh(ID3D11Device* Device, const std::vector<TVertex>& Vertices, const std::vector<UINT>& Indices);

		Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;

		const UINT VertexCount;
		const UINT IndexCount;
		const UINT Stride;
	};

	template<typename TVertex, typename TEnable>
	UMesh<TVertex, TEnable>::UMesh(ID3D11Device* Device, const std::vector<TVertex>& Vertices, const std::vector<UINT>& Indices)
		: VertexCount(Vertices.size()), IndexCount(Indices.size()), Stride(sizeof(TVertex))
	{
		CreateMesh(Device, Vertices, Indices);
	}

	template<typename TVertex, typename TEnable>
	void UMesh<TVertex, TEnable>::Bind(ID3D11DeviceContext* DeviceContext) const
	{
		UINT Offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &Stride, &Offset);

		DeviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	template<typename TVertex, typename TEnable>
	std::vector<D3D11_INPUT_ELEMENT_DESC> UMesh<TVertex, TEnable>::GetInputLayoutDesc() const
	{
		return TVertex::GetInputLayoutDesc();
	}

	template<typename TVertex, typename TEnable>
	UINT UMesh<TVertex, TEnable>::GetVertexCount() const
	{
		return VertexCount;
	}

	template<typename TVertex, typename TEnable>
	UINT UMesh<TVertex, TEnable>::GetIndexCount() const
	{
		return IndexCount;
	}

	template<typename TVertex, typename TEnable>
	UINT UMesh<TVertex, TEnable>::GetStride() const
	{
		return Stride;
	}

	template<typename TVertex, typename TEnable>
	void UMesh<TVertex, TEnable>::CreateMesh(ID3D11Device* Device, const std::vector<TVertex>& Vertices, const std::vector<UINT>& Indices)
	{
		D3D11_BUFFER_DESC VertexBufferDesc = {};
		VertexBufferDesc.ByteWidth = static_cast<UINT>(VertexCount * Stride);
		VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = 0;
		VertexBufferDesc.MiscFlags = 0;
		VertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA VertexBufferSRD = { Vertices.data() };

		Device->CreateBuffer(&VertexBufferDesc, &VertexBufferSRD, VertexBuffer.GetAddressOf());

		D3D11_BUFFER_DESC IndexBufferDesc = {};
		IndexBufferDesc.ByteWidth = static_cast<UINT>(IndexCount * sizeof(UINT));
		IndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = 0;
		IndexBufferDesc.MiscFlags = 0;
		IndexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA IndexBufferSRD = { Indices.data() };

		Device->CreateBuffer(&IndexBufferDesc, &IndexBufferSRD, IndexBuffer.GetAddressOf());
	}

} // namespace DXD
