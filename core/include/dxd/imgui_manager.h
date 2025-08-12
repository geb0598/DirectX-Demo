#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace DXD
{

    class UImGuiManager
    {
    public:
        ~UImGuiManager()
        {
            ImGui::DestroyContext(); 
        }

        UImGuiManager() 
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
        }
    };

} // namespace DXD