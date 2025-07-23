#include "dxd.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	dxd::UWindow window(1024, 1024, L"Triangle");

    dxd::URenderer::GetInstance().Create(window.GetHWND());

    std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
    {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    std::filesystem::path resources(RESOURCES_PATH);

    dxd::SHADER_DESC shaderdesc = {};
    shaderdesc.vsFilePath = resources / "vertex.hlsl";
    shaderdesc.psFilePath = resources / "pixel.hlsl";
    shaderdesc.layout = layout;

    auto shader = std::make_shared<dxd::UShader>(dxd::URenderer::GetInstance().GetDevice(), shaderdesc);

    std::vector<dxd::Vertex> vertices(3);
	vertices[0].Position = { -0.5, -0.5, 0 };
	vertices[1].Position = {  0.5, -0.5, 0 };
	vertices[2].Position = {  0,    0.5, 0 };
    // NOTE:
    // std::vector<UINT> indices = { 0, 1, 2 }; 
    // Backface culling apllied to counter-clockwise vertices unlike OpenGL
    std::vector<UINT> indices = { 0, 2, 1 };

    auto mesh = std::make_shared<dxd::UMesh>(dxd::URenderer::GetInstance().GetDevice(), vertices, indices);

    dxd::UObject object;
    object.AddComponent<dxd::UMeshRendererComponent>(std::move(mesh), std::move(shader));

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
		dxd::URenderer::GetInstance().Prepare();

		object.GetComponent<dxd::UMeshRendererComponent>()->Render(dxd::URenderer::GetInstance().GetDeviceContext());

		dxd::URenderer::GetInstance().Render();
    }
}
