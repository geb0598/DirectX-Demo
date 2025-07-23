#include "dxd/mesh_renderer_component.h"

namespace dxd
{

	UMeshRendererComponent::UMeshRendererComponent(UObject* object, std::shared_ptr<UMesh> mesh, std::shared_ptr<UShader> shader)
		: UComponentImpl(object), Mesh(mesh), Shader(shader) {}

	void UMeshRendererComponent::Render(ID3D11DeviceContext* deviceContext)
	{
		// TODO: get transform info
		//		 update constant buffer of shader

		Shader->Bind(deviceContext);

		Mesh->Bind(deviceContext);

		deviceContext->DrawIndexed(Mesh->GetIndexCount(), 0, 0);
	}

} // namespace dxd