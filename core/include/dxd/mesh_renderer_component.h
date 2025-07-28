#pragma once

#include "dxd/mesh.h"
#include "dxd/shader.h"
#include "dxd/buffer.h"

#include "dxd/component.h"

namespace DXD
{

	class UMeshRendererComponent : public UComponent
	{
	public:
		~UMeshRendererComponent() = default;

		UMeshRendererComponent(UGameObject* GameObject, std::shared_ptr<IMesh> Mesh, 
			std::shared_ptr<IShader> VertexShader, std::shared_ptr<IShader> PixelShader);

		UMeshRendererComponent(const UMeshRendererComponent&) = delete;
		UMeshRendererComponent& operator=(const UMeshRendererComponent&) = delete;

		UMeshRendererComponent(UMeshRendererComponent&&) noexcept = default;
		UMeshRendererComponent& operator=(UMeshRendererComponent&&) noexcept = default;

		void Render(ID3D11DeviceContext* DeviceContext,
			std::vector<std::unique_ptr<IBufferDataWrapper>> VSConstantBuffers,
			std::vector<std::unique_ptr<IBufferDataWrapper>> PSConstantBuffers);

	private:
		std::shared_ptr<IMesh> Mesh;
		std::shared_ptr<IShader> VertexShader;
		std::shared_ptr<IShader> PixelShader;
	};

} // namespace DXD
