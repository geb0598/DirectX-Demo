#include <iostream>

#include "dxd.h"
#include "utils.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    dxd::UWindow Window(1024, 1024, L"04 mesh loader");

    auto& Renderer = dxd::URenderer::GetInstance(Window.GetHWindow());
    auto Device = Renderer.GetDevice();
    auto DeviceContext = Renderer.GetDeviceContext();

    std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    std::filesystem::path Resources(RESOURCES_PATH);

    dxd::SHADER_DESC ShaderDesc = {};
    ShaderDesc.vsFilePath = Resources / "vertex.hlsl";
    ShaderDesc.psFilePath = Resources / "pixel.hlsl";
    ShaderDesc.layout = layout;

    auto Shader = std::make_shared<dxd::UShader>(Device, ShaderDesc);

    auto [Vertices, Indices] = dxd::OBJMeshLoader::GetInstance().LoadMesh(Resources / "teapot.obj");

    auto Mesh = std::make_shared<dxd::UMesh>(Device, Vertices, Indices);

    dxd::UGameObject Object;
    Object.AddComponent<dxd::UMeshRendererComponent>(Mesh, Shader);
    Object.GetComponent<dxd::UTransformComponent>()->SetScale({ 0.1f, 0.1f, 0.1f });

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

		dxd::VS_CONSTANT_BUFFER_DATA VSConstantBufferData = {};
		VSConstantBufferData.World = Object.GetComponent<dxd::UTransformComponent>()->GetWorldMatrix();
		dxd::PS_CONSTANT_BUFFER_DATA PSConstantBufferData = {};
		Object.GetComponent<dxd::UMeshRendererComponent>()->Render(DeviceContext, VSConstantBufferData, PSConstantBufferData);

        Renderer.Render();
    }
}
