#include "dxd/mesh.h"

namespace dxd
{

	UMesh::UMesh(ID3D11Device* Device, const std::vector<Vertex>& Vertices, const std::vector<UINT>& Indices)
		: VertexCount(Vertices.size()), IndexCount(Indices.size()), Stride(sizeof(Vertex))
	{
		CreateMesh(Device, Vertices, Indices);
	}

	void UMesh::Bind(ID3D11DeviceContext* DeviceContext)
	{
		UINT Offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &Stride, &Offset);

		DeviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	UINT UMesh::GetVertexCount() const
	{
		return VertexCount;
	}

	UINT UMesh::GetIndexCount() const
	{
		return IndexCount;
	}

	UINT UMesh::GetStride() const
	{
		return Stride;
	}

	void UMesh::CreateMesh(ID3D11Device* Device, const std::vector<Vertex>& Vertices, const std::vector<UINT>& Indices)
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

} // namespace dxd