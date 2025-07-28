#include <iostream>

#include "dxd.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    DXD::UWindow Window(800, 800, L"00 experimental build");

    auto& Renderer = DXD::URenderer::GetInstance(Window.GetHWindow());
    auto Device = Renderer.GetDevice();
    auto DeviceContext = Renderer.GetDeviceContext();

    std::filesystem::path Resources(RESOURCES_PATH);

    auto VertexShader = std::make_shared<DXD::UVertexShader<DXD::FVERTEX_PN>>(Device, Resources / "vertex.hlsl");
    auto PixelShader = std::make_shared<DXD::UPixelShader>(Device, Resources / "pixel.hlsl");

    auto [Vertices, Indices] = DXD::UObjMeshLoader::GetInstance().LoadMesh<DXD::FVERTEX_PN>(Resources / "teapot.obj");
    auto Mesh = std::make_shared<DXD::UMesh<DXD::FVERTEX_PN>>(Device, Vertices, Indices);

    DXD::UGameObject Object;
    Object.AddComponent<DXD::UMeshRendererComponent>(Mesh, VertexShader, PixelShader);
    Object.GetComponent<DXD::UTransformComponent>()->SetScale({ 0.1f, 0.1f, 0.1f });

    DXD::UGameObject Camera;
    Camera.AddComponent<DXD::UCameraComponent>();
    Camera.GetComponent<DXD::UCameraComponent>()->SetAspectRatio(800.0f / 800.0f);
    Camera.GetComponent<DXD::UTransformComponent>()->Translate({ 0.0f, 0.0f, -3.0f });

    DXD::UGameObject Lighting;
    Lighting.GetComponent<DXD::UTransformComponent>()->Translate({ 0.0f, 5.0f, 0.0f });

    const int TargetFPS = 30;
    const double TargetFrameTime = 1000.0 / TargetFPS;

    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);

    LARGE_INTEGER StartTime, EndTime;
    double ElapsedTime = 0.0f;

    bool bIsExit = false;
    while (bIsExit == false)
    {
        QueryPerformanceCounter(&StartTime);

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
        //------------------------------- Start Rendering ----------------------------------//
        Renderer.Prepare();

        DXD::CB::TRANSFORM_DATA TransformData = {};
        TransformData.World = Object.GetComponent<DXD::UTransformComponent>()->GetWorldMatrix();
        TransformData.View = Camera.GetComponent<DXD::UCameraComponent>()->GetViewMatrix();
        TransformData.Projection = Camera.GetComponent<DXD::UCameraComponent>()->GetProjectionMatrix();

        DXD::CB::MATERIAL_DATA MaterialData = {};
        MaterialData.M_DiffuseColor = { 1.0f, 0.0f, 0.0f, 1.0f };
        MaterialData.M_SpecularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        MaterialData.M_AmbientColor = { 0.8f, 0.2f, 0.2f, 1.0f };
        MaterialData.Shininess = { 30.0f, 0.0f, 0.0f, 0.0f };

        DXD::CB::LIGHTING_DATA LightingData = {};
	    LightingData.L_DiffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        LightingData.L_SpecularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        LightingData.L_AmbientColor = { 0.1f, 0.1f, 0.1f, 1.0f };
        LightingData.LightingPosition = Lighting.GetComponent<DXD::UTransformComponent>()->GetPosition4D();
        LightingData.CameraPosition = Camera.GetComponent<DXD::UTransformComponent>()->GetPosition4D();

        Object.GetComponent<DXD::UMeshRendererComponent>()->Render(DeviceContext, 
            DXD::WrapBufferDataPack(TransformData), 
            DXD::WrapBufferDataPack(MaterialData, LightingData));

        Renderer.Render();
        //------------------------------- Finish Rendering --------------------------------//

        Object.GetComponent<DXD::UTransformComponent>()->Rotate({ 0.0f, 1.0f, 0.0f }, ElapsedTime * DirectX::XM_PI / 1000.0f);

        do
        {
            Sleep(0);

            QueryPerformanceCounter(&EndTime);

            ElapsedTime = (EndTime.QuadPart - StartTime.QuadPart) * 1000.0 / Frequency.QuadPart;
        } while (ElapsedTime < TargetFrameTime);
    }
}
