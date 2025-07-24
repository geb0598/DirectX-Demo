#include "dxd.h"
#include "utils.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    dxd::UWindow Window(1024, 1024, L"Triangle");

    auto& Renderer = dxd::URenderer::GetInstance(Window.GetHWindow());
    auto Device = Renderer.GetDevice();
    auto DeviceContext = Renderer.GetDeviceContext();

    std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    std::filesystem::path resources(RESOURCES_PATH);

    dxd::SHADER_DESC shaderdesc = {};
    shaderdesc.vsFilePath = resources / "vertex.hlsl";
    shaderdesc.psFilePath = resources / "pixel.hlsl";
    shaderdesc.layout = layout;

    auto shader = std::make_shared<dxd::UShader>(Device, shaderdesc);

    std::vector<dxd::Vertex> vertices(3);
    vertices[0].Position = { -0.5, -0.5, 0 };
    vertices[1].Position = { 0.5, -0.5, 0 };
    vertices[2].Position = { 0,    0.5, 0 };
    // NOTE:
    // std::vector<UINT> indices = { 0, 1, 2 }; 
    // Backface culling apllied to counter-clockwise vertices unlike OpenGL
    std::vector<UINT> indices = { 0, 2, 1 };

    auto mesh = std::make_shared<dxd::UMesh>(Device, vertices, indices);

    std::vector<dxd::UGameObject> objects;
    for (int i = 0; i < 100; ++i)
    {
        dxd::UGameObject object;
		object.AddComponent<dxd::UMeshRendererComponent>(mesh, shader);
        float x = utils::RandomGenerator::GetInstance().GetDouble(-1.0, 1.0);
        float y = utils::RandomGenerator::GetInstance().GetDouble(-1.0, 1.0);
        float z = utils::RandomGenerator::GetInstance().GetDouble(-1.0, 1.0);
        float scale = utils::RandomGenerator::GetInstance().GetDouble(0.1, 0.3);
		object.GetComponent<dxd::UTransformComponent>()->SetPosition({ x, y, z });
		object.GetComponent<dxd::UTransformComponent>()->SetScale({ scale, scale, scale });

        objects.push_back(std::move(object));
    }

    bool bIsExit = false;
    while (bIsExit == false)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                bIsExit = true;
                break;
            }
        }
	    Renderer.Prepare();

        for (auto& object : objects)
        {
			dxd::VS_CONSTANT_BUFFER_DATA VSConstantBufferData = {};
			VSConstantBufferData.World = object.GetComponent<dxd::UTransformComponent>()->GetWorldMatrix();
			dxd::PS_CONSTANT_BUFFER_DATA PSConstantBufferData = {};
			object.GetComponent<dxd::UMeshRendererComponent>()->Render(DeviceContext, VSConstantBufferData, PSConstantBufferData);
        }

        Renderer.Render();
    }
}
