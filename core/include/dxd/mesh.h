#pragma once

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <vector>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <wrl.h>

#include "vertex.h"

namespace dxd
{

	class UMesh
	{
	public:
		~UMesh() = default;

		UMesh(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices);

		void Bind(ID3D11DeviceContext* deviceContext);

		UINT GetVertexCount() const;
		UINT GetIndexCount() const;
		UINT GetStride() const;

	private:
		void CreateMesh(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices);

		Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer = nullptr;
		UINT VertexCount;
		UINT IndexCount;
		UINT Stride;
	};

} // namespace dxd
