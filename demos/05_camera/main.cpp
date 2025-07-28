#include <iostream>

#include "dxd.h"
#include "utils.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    DXD::UWindow Window(1600, 900, L"04 mesh loader");

    auto& Renderer = DXD::URenderer::GetInstance(Window.GetHWindow());
    auto Device = Renderer.GetDevice();
    auto DeviceContext = Renderer.GetDeviceContext();

    std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    DXD::UGameObject Camera;
    Camera.AddComponent<DXD::UCameraComponent>();
    Camera.GetComponent<DXD::UTransformComponent>()->Translate({ 0.0f, 0.0f, 10.0f });

    std::filesystem::path Resources(RESOURCES_PATH);

    DXD::SHADER_DESC ShaderDesc = {};
    ShaderDesc.vsFilePath = Resources / "vertex.hlsl";
    ShaderDesc.psFilePath = Resources / "pixel.hlsl";
    ShaderDesc.layout = layout;

    auto Shader = std::make_shared<DXD::UShader>(Device, ShaderDesc);

    auto [Vertices, Indices] = DXD::OBJMeshLoader::GetInstance().LoadMesh(Resources / "teapot.obj");

    auto Mesh = std::make_shared<DXD::UMesh>(Device, Vertices, Indices);

    DXD::UGameObject Object;
    Object.AddComponent<DXD::UMeshRendererComponent>(Mesh, Shader);
    Object.GetComponent<DXD::UTransformComponent>()->SetScale({ 0.1f, 0.1f, 0.1f });

    bool bIsExit = false;
    while (bIsExit == false)
    {
        MSG Message;
        while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);

            if (Message.message == WM_QUIT)
            {
                bIsExit = true;
                break;
            }
        }
        Renderer.Prepare();

		DXD::VS_CONSTANT_BUFFER_DATA VSConstantBufferData = {};
		VSConstantBufferData.World = Object.GetComponent<DXD::UTransformComponent>()->GetWorldMatrix();
        VSConstantBufferData.View = Camera.GetComponent<DXD::UCameraComponent>()->GetViewMatrix();
        VSConstantBufferData.Projection = Camera.GetComponent<DXD::UCameraComponent>()->GetProjectionMatrix();
		DXD::PS_CONSTANT_BUFFER_DATA PSConstantBufferData = {};
		Object.GetComponent<DXD::UMeshRendererComponent>()->Render(DeviceContext, VSConstantBufferData, PSConstantBufferData);

        Renderer.Render();
    }
}
