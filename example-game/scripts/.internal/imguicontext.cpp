#include "imgui/imgui.h"

extern "C" __declspec(dllexport) void initializeImGuiContext(ImGuiContext* context) {
    ImGui::SetCurrentContext(context);
}
