#include "editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "scene.h"
#include "inputsjustpressed.h"
#include "framebuffer.h"
#include <glad/glad.h>

void Editor::updateRender(Scene& scene, const InputsJustPressed& inputs, const Framebuffer& gBuffer) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Select object
    if (inputs.checkMouseButton(GLFW_MOUSE_BUTTON_LEFT) && !ImGui::IsAnyItemHovered()) {
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, mouseX, mouseY); // TODO y-up vs y-down?
        int index{ (int)gBuffer.samplePixel<float>(mouseX, mouseY, 0)[3] };
    }

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