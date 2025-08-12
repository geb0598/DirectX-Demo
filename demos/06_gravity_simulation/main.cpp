#include <iostream>

#include "dxd.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    //------------------------------- Window Setup ----------------------------------//
    DXD::UWindow Window(1600, 1600, L"00 experimental build");

    //------------------------------- Renderer Setup --------------------------------//
    auto& Renderer = DXD::URenderer::GetInstance(Window.GetHWindow());
    auto Device = Renderer.GetDevice();
    auto DeviceContext = Renderer.GetDeviceContext();

    std::filesystem::path Resources(RESOURCES_PATH);

    //------------------------------- Load Shader ------------------------------------//
    auto VertexShader = std::make_shared<DXD::UVertexShader<DXD::FVERTEX_PN>>(Device, Resources / "vertex.hlsl");
    auto PixelShader = std::make_shared<DXD::UPixelShader>(Device, Resources / "pixel.hlsl");

    //------------------------------- Load Mesh --------------------------------------//
    auto [Vertices, Indices] = DXD::UObjMeshLoader::GetInstance().LoadMesh<DXD::FVERTEX_PN>(Resources / "teapot.obj");
    auto Mesh = std::make_shared<DXD::UMesh<DXD::FVERTEX_PN>>(Device, Vertices, Indices);

    //------------------------------- Object Setup -----------------------------------//
    DXD::UGameObject Object;
    Object.AddComponent<DXD::UMeshRendererComponent>(Mesh, VertexShader, PixelShader);
    Object.GetComponent<DXD::UTransformComponent>()->SetScale({ 0.1f, 0.1f, 0.1f });

    //------------------------------- Camera Setup -----------------------------------//
    DXD::UGameObject Camera;
    Camera.AddComponent<DXD::UCameraComponent>();
    Camera.GetComponent<DXD::UCameraComponent>()->SetAspectRatio(800.0f / 800.0f);
    Camera.GetComponent<DXD::UTransformComponent>()->Translate({ 0.0f, 0.0f, -3.0f });

    //------------------------------- Lighting Setup ---------------------------------//
    DXD::UGameObject Lighting;
    Lighting.GetComponent<DXD::UTransformComponent>()->Translate({ 0.0f, 5.0f, 0.0f });

    //------------------------------- ImGui Setup ------------------------------------//
    DXD::UImGuiManager ImGuiManager;
    ImGui_ImplWin32_Init(reinterpret_cast<void*>(Window.GetHWindow()));
    ImGui_ImplDX11_Init(Device, DeviceContext);

    //------------------------------- Main Loop Setup --------------------------------//
    const int TargetFPS = 60;
    const double TargetFrameTime = 1000.0 / TargetFPS;

    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);

    LARGE_INTEGER StartTime, EndTime, PreviousTime;
    QueryPerformanceCounter(&PreviousTime);
    double ElapsedTime = 0.0f;

    std::pair<int, int> MousePosition;

    bool bIsExit = false;
    while (bIsExit == false)
    {
        QueryPerformanceCounter(&StartTime);

        float DeltaTime = static_cast<float>(StartTime.QuadPart - PreviousTime.QuadPart) / Frequency.QuadPart;
        PreviousTime = StartTime;

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
        //------------------------------- Start Control ------------------------------------//
        auto CameraTransform = Camera.GetComponent<DXD::UTransformComponent>();

        float Speed = 1.0f;
        if (Window.Keyboard.IsKeyPressed('W'))
        {
            CameraTransform->TranslateLocal({ 0.0f, 0.0f, Speed * DeltaTime });
        }
        if (Window.Keyboard.IsKeyPressed('A'))
        {
            CameraTransform->TranslateLocal({ -Speed * DeltaTime, 0.0f, 0.0f });
        }
        if (Window.Keyboard.IsKeyPressed('S'))
        {
            CameraTransform->TranslateLocal({ 0.0f, 0.0f, -Speed * DeltaTime });
        }
        if (Window.Keyboard.IsKeyPressed('D'))
        {
            CameraTransform->TranslateLocal({ Speed * DeltaTime, 0.0f, 0.0f });
        }

        float XSensitivity = 0.1f;
        float YSensitivity = 0.1f;
        std::pair<int, int> NewMousePosition = Window.Mouse.GetPosition();

        int DeltaX = NewMousePosition.first - MousePosition.first;
        int DeltaY = NewMousePosition.second - MousePosition.second;

        CameraTransform->Rotate(DeltaY * YSensitivity * DeltaTime, DeltaX * XSensitivity * DeltaTime, 0.0f);

        MousePosition = NewMousePosition;
        //------------------------------- Finish Control -----------------------------------//

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
        //------------------------------- Finish Rendering --------------------------------//

        //------------------------------- Start GUI Rendering -----------------------------//

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Performance Monitor");
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Text("Delta Time: %.3f ms", io.DeltaTime * 1000.0f);
        ImGui::End();

        if (CameraTransform)
        {
            ImGui::Begin("Camera Info");

            DirectX::XMFLOAT3 Position = CameraTransform->GetPosition();
            ImGui::Text("Position: X=%.2f, Y=%.2f, Z=%.2f", Position.x, Position.y, Position.z);

            DirectX::XMFLOAT4 Rotation = CameraTransform->GetRotation();
            ImGui::Text("Rotation(Quaternion): X=%.2f, Y=%.2f, Z=%.2f, W=%.2f", Rotation.x, Rotation.y, Rotation.z, Rotation.w);

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        //------------------------------- Finish GUI Rendering ----------------------------//
        Renderer.Render();

        Object.GetComponent<DXD::UTransformComponent>()->Rotate({ 0.0f, 1.0f, 0.0f }, ElapsedTime * DirectX::XM_PI / 1000.0f);

        do
        {
            Sleep(0);

            QueryPerformanceCounter(&EndTime);

            ElapsedTime = (EndTime.QuadPart - StartTime.QuadPart) * 1000.0 / Frequency.QuadPart;
        } while (ElapsedTime < TargetFrameTime);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
}
