#pragma once

#include "component.h"
#include "mesh.h"
#include "object.h"
#include "shader.h"

namespace dxd
{
	class UMeshRendererComponent : public UComponentImpl
	{
	public:
		~UMeshRendererComponent() = default;

		UMeshRendererComponent(const UMeshRendererComponent&) = delete;
		UMeshRendererComponent(UMeshRendererComponent&&) noexcept = default;

		UMeshRendererComponent& operator=(const UMeshRendererComponent&) = delete;
		UMeshRendererComponent& operator=(UMeshRendererComponent&&) noexcept = default;

		UMeshRendererComponent(UGameObject* GameObject, std::shared_ptr<UMesh> Mesh, std::shared_ptr<UShader> Shader);

		void Render(ID3D11DeviceContext* DeviceContext,
			const VS_CONSTANT_BUFFER_DATA& VSConstantBufferData,
			const PS_CONSTANT_BUFFER_DATA& PSConstantBufferData);

	private:
		std::shared_ptr<UMesh> Mesh;
		std::shared_ptr<UShader> Shader;
	};

} // namespace dxd
