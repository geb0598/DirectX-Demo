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

		UMesh(const UMesh&) = delete;
		UMesh(UMesh&&) noexcept = default;

		UMesh& operator=(const UMesh&) = delete;
		UMesh& operator=(UMesh&&) noexcept = default;

		UMesh(ID3D11Device* Device, const std::vector<FVertex>& Vertices, const std::vector<UINT>& Indices);

		void Bind(ID3D11DeviceContext* DeviceContext);

		UINT GetVertexCount() const;
		UINT GetIndexCount() const;
		UINT GetStride() const;

	private:
		void CreateMesh(ID3D11Device* Device, const std::vector<FVertex>& Vertices, const std::vector<UINT>& Indices);

		Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer = nullptr;
		UINT VertexCount;
		UINT IndexCount;
		UINT Stride;
	};

} // namespace dxd
