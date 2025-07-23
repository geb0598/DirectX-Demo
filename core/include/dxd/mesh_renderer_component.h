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

		UMeshRendererComponent(UObject* object, std::shared_ptr<UMesh> mesh, std::shared_ptr<UShader> shader);

		void Render(ID3D11DeviceContext* deviceContext);
		
	private:
		std::shared_ptr<UMesh> Mesh;
		std::shared_ptr<UShader> Shader;
	};

} // namespace dxd
