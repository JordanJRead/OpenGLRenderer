#include "editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "scene.h"

void Editor::updateRender(Scene& scene) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Scene Lighting");
    ImGui::ColorEdit3("Sunlight Colour", (float*)&scene.getDirectionalLight().mColour);
    ImGui::ColorEdit3("Ambient Colour", (float*)&scene.getAmbientLightColour());
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::destroyUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}