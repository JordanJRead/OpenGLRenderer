#include "editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "scene.h"
#include "inputs.h"
#include "framebuffer.h"
#include <glad/glad.h>
#include <iostream>
#include "imgui/imgui_stdlib.h"
#include "nlohmann/json.hpp"

void Editor::updateRender(Scene& scene, GLFWwindow* window, const Inputs& inputs, const Framebuffer& gBuffer) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Select object
    if (inputs.checkMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && !ImGui::IsAnyItemHovered()) {
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        mouseY = scene.getCamera().getHeight() - 1 - mouseY;
        mSelectedObjectIndex = (int)gBuffer.samplePixel((int)mouseX, (int)mouseY, 0)[3];
    }

    ImGui::Begin("Scene Lighting");
    ImGui::ColorEdit3("Sunlight Colour", (float*)&scene.getDirectionalLight().mColour);
    ImGui::ColorEdit3("Ambient Colour", (float*)&scene.getAmbientLightColour());
    ImGui::End();

    // Inspector
    ImGui::Begin("Object Editor");
    if (scene.isValidObjectIndex(mSelectedObjectIndex)) {
        SceneObject& object{ scene.getObject(mSelectedObjectIndex) };
        ImGui::PushID(&object);
        ImGui::InputText("", &object.getName());
        ImGui::PopID();

        object.getTransform().renderUI();
    }

    else {
        ImGui::Text("No object selected");
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::destroyUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}