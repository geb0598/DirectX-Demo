#include "dxd/mesh.h"

namespace dxd
{

	UMesh::UMesh(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices)
		: VertexCount(vertices.size()), IndexCount(indices.size()), Stride(sizeof(Vertex))
	{
		CreateMesh(device, vertices, indices);
	}

	void UMesh::Bind(ID3D11DeviceContext* deviceContext)
	{
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &Stride, &offset);

		deviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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

	void UMesh::CreateMesh(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices)
	{
		D3D11_BUFFER_DESC vertexbufferdesc = {};
		vertexbufferdesc.ByteWidth = static_cast<UINT>(VertexCount * Stride);
		vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexbufferdesc.CPUAccessFlags = 0;
		vertexbufferdesc.MiscFlags = 0;
		vertexbufferdesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices.data() };

		device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, VertexBuffer.GetAddressOf());

		D3D11_BUFFER_DESC indexbufferdesc = {};
		indexbufferdesc.ByteWidth = static_cast<UINT>(IndexCount * sizeof(UINT));
		indexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexbufferdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexbufferdesc.CPUAccessFlags = 0;
		indexbufferdesc.MiscFlags = 0;
		indexbufferdesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexbufferSRD = { indices.data() };

		device->CreateBuffer(&indexbufferdesc, &indexbufferSRD, IndexBuffer.GetAddressOf());
	}

} // namespace dxd