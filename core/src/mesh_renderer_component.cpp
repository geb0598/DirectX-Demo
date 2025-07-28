#include "dxd/component.h"
#include "dxd/transform_component.h"
#include "dxd/mesh_renderer_component.h"

namespace DXD
{

	UMeshRendererComponent::UMeshRendererComponent(UGameObject* GameObject, std::shared_ptr<IMesh> Mesh,
		std::shared_ptr<IShader> VertexShader, std::shared_ptr<IShader> PixelShader)
		: UComponent(GameObject), Mesh(Mesh), VertexShader(VertexShader), PixelShader(PixelShader) {}

	void UMeshRendererComponent::Render(ID3D11DeviceContext* DeviceContext,
		std::vector<std::unique_ptr<IBufferDataWrapper>> VSConstantBuffers,
		std::vector<std::unique_ptr<IBufferDataWrapper>> PSConstantBuffers)
	{
		std::vector<std::string> VSBufferNames;
		for (const auto& VSConstantBuffer : VSConstantBuffers)
		{
			VertexShader->UpdateConstantBuffer(DeviceContext,
				VSConstantBuffer->GetBufferDataName(), VSConstantBuffer->GetBufferData());
			VSBufferNames.push_back(VSConstantBuffer->GetBufferDataName());
		}
		VertexShader->Bind(DeviceContext, VSBufferNames);

		std::vector<std::string> PSBufferNames;
		for (const auto& PSConstantBuffer : PSConstantBuffers)
		{
			PixelShader->UpdateConstantBuffer(DeviceContext,
				PSConstantBuffer->GetBufferDataName(), PSConstantBuffer->GetBufferData());
			PSBufferNames.push_back(PSConstantBuffer->GetBufferDataName());
		}
		PixelShader->Bind(DeviceContext, PSBufferNames);

		Mesh->Bind(DeviceContext);

		DeviceContext->DrawIndexed(Mesh->GetIndexCount(), 0, 0);
	}

} // namespace DXD