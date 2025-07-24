#include "dxd.h"
#include "utils.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    dxd::UWindow Window(1024, 1024, L"03 multiple triangles");

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

    std::vector<dxd::FVertex> Vertices(3);
    Vertices[0].Position = { -0.5, -0.5, 0 };
    Vertices[1].Position = { 0.5, -0.5, 0 };
    Vertices[2].Position = { 0,    0.5, 0 };
    // NOTE:
    // std::vector<UINT> Indices = { 0, 1, 2 }; 
    // Backface culling apllied to counter-clockwise Vertices unlike OpenGL
    std::vector<UINT> Indices = { 0, 2, 1 };

    auto Mesh = std::make_shared<dxd::UMesh>(Device, Vertices, Indices);

    std::vector<dxd::UGameObject> Objects;
    for (int i = 0; i < 100; ++i)
    {
        dxd::UGameObject Object;
		Object.AddComponent<dxd::UMeshRendererComponent>(Mesh, Shader);
        float x = utils::RandomGenerator::GetInstance().GetDouble(-1.0, 1.0);
        float y = utils::RandomGenerator::GetInstance().GetDouble(-1.0, 1.0);
        float z = utils::RandomGenerator::GetInstance().GetDouble(-1.0, 1.0);
        float scale = utils::RandomGenerator::GetInstance().GetDouble(0.1, 0.3);
		Object.GetComponent<dxd::UTransformComponent>()->SetPosition({ x, y, z });
		Object.GetComponent<dxd::UTransformComponent>()->SetScale({ scale, scale, scale });

        Objects.push_back(std::move(Object));
    }

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

        for (auto& Object : Objects)
        {
			dxd::VS_CONSTANT_BUFFER_DATA VSConstantBufferData = {};
			VSConstantBufferData.World = Object.GetComponent<dxd::UTransformComponent>()->GetWorldMatrix();
			dxd::PS_CONSTANT_BUFFER_DATA PSConstantBufferData = {};
			Object.GetComponent<dxd::UMeshRendererComponent>()->Render(DeviceContext, VSConstantBufferData, PSConstantBufferData);
        }

        Renderer.Render();
    }
}
