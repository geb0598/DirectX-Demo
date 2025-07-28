#include "dxd.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    DXD::UWindow Window(1024, 1024, L"Triangle");

    auto& Renderer = DXD::URenderer::GetInstance(Window.GetHWindow());
    auto Device = Renderer.GetDevice();
    auto DeviceContext = Renderer.GetDeviceContext();

    std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    std::filesystem::path resources(RESOURCES_PATH);

    DXD::SHADER_DESC shaderdesc = {};
    shaderdesc.vsFilePath = resources / "vertex.hlsl";
    shaderdesc.psFilePath = resources / "pixel.hlsl";
    shaderdesc.layout = layout;

    auto shader = std::make_shared<DXD::UShader>(Device, shaderdesc);

    std::vector<DXD::FVertex> vertices(3);
    vertices[0].Position = { -0.5, -0.5, 0 };
    vertices[1].Position = { 0.5, -0.5, 0 };
    vertices[2].Position = { 0,    0.5, 0 };
    // NOTE:
    // std::vector<UINT> indices = { 0, 1, 2 }; 
    // Backface culling apllied to counter-clockwise vertices unlike OpenGL
    std::vector<UINT> indices = { 0, 2, 1 };

    auto mesh = std::make_shared<DXD::UMesh>(Device, vertices, indices);

    DXD::UGameObject object;
    object.AddComponent<DXD::UMeshRendererComponent>(mesh, shader);

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

        DXD::VS_CONSTANT_BUFFER_DATA VSConstantBufferData = {};
        VSConstantBufferData.World = object.GetComponent<DXD::UTransformComponent>()->GetWorldMatrix();
        DXD::PS_CONSTANT_BUFFER_DATA PSConstantBufferData = {};
        object.GetComponent<DXD::UMeshRendererComponent>()->Render(DeviceContext, VSConstantBufferData, PSConstantBufferData);

        Renderer.Render();
    }
}
