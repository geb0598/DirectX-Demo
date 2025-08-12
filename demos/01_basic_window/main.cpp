#include "dxd.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    DXD::UWindow Window(1024, 1024, L"Hello Window!");

    auto& Renderer = DXD::URenderer::GetInstance(Window.GetHWindow());
    auto Device = Renderer.GetDevice();
    auto DeviceContext = Renderer.GetDeviceContext();

    DXD::UImGuiManager ImGuiManager;

    ImGui_ImplWin32_Init(reinterpret_cast<void*>(Window.GetHWindow()));
    ImGui_ImplDX11_Init(Device, DeviceContext);

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

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("ImGui Window");
        ImGui::Text("Mouse Position: (%d, %d)", Window.Mouse.GetXPosition(), Window.Mouse.GetYPosition());
        auto Character = Window.Keyboard.ReadChar();
        ImGui::End();

        ImGui::Begin("Performance Monitor");
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Text("Delta Time: %.3f ms", io.DeltaTime * 1000.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        Renderer.Render();
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
}