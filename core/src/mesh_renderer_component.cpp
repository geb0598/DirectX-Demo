#include "dxd/mesh_renderer_component.h"
#include "dxd/transform_component.h"

namespace dxd
{

	UMeshRendererComponent::UMeshRendererComponent(
		UGameObject* GameObject, std::shared_ptr<UMesh> Mesh, std::shared_ptr<UShader> Shader)
		: UComponentImpl(GameObject), Mesh(Mesh), Shader(Shader) {}

	void UMeshRendererComponent::Render(
		ID3D11DeviceContext* DeviceContext, 
		const VS_CONSTANT_BUFFER_DATA& VSConstantBufferData, 
		const PS_CONSTANT_BUFFER_DATA& PSConstantBufferData)
	{
		// TODO: get Transform info
		//		 update constant buffer of shader

		/*
		auto Transform = GetGameObject()->GetComponent<UTransformComponent>();
		if (Transform != nullptr)
		{
			VS_CONSTANT_BUFFER_DATA VSConstantBufferData = {};
			VSConstantBufferData.World = Transform->GetWorldMatrix();
			Shader->UpdateVSConstants(DeviceContext, VSConstantBufferData);
		}
		*/

		Shader->UpdateVSConstants(DeviceContext, VSConstantBufferData);

		Shader->UpdatePSConstants(DeviceContext, PSConstantBufferData);

		Shader->Bind(DeviceContext);

		Mesh->Bind(DeviceContext);

		DeviceContext->DrawIndexed(Mesh->GetIndexCount(), 0, 0);
	}

} // namespace dxd