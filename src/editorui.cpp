#include "editorui.h"
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
#include "rendersettings.h"
#include "editor.h"
#include "sceneobject.h"

EditorUI::EditorUI() {
    //ImGuiStyle& style{ ImGui::GetStyle() };
    //style.Colors[ImGuiCol_Text] = ImVec4{ 0, 0, 1, 1 };
    //style.Colors[ImGuiCol_TextDisabled] = ImVec4{ 0, 0, 1, 1 };
    //style.Colors[ImGuiCol_WindowBg] = ImVec4{0, 0, 1, 1};
}

glm::ivec2 EditorUI::updateRender(const Framebuffer* const outputFramebuffer, Editor& editor) {
    Framebuffer::bind(outputFramebuffer);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::Begin("Scene Lighting");
    ImGui::ColorEdit3("Sunlight Colour", (float*)&editor.mScene.getDirectionalLight().mColour);
    ImGui::ColorEdit3("Ambient Colour", (float*)&editor.mScene.getAmbientLightColour());
    ImGui::End();

    ImGui::Begin("Render Settings");
    ImGui::ColorEdit3("Highlight Colour", (float*)&editor.mRenderSettings.mValue.mHighlightColour);
    ImGui::DragFloat("Point Light Size", &editor.mRenderSettings.mValue.mPointLightRenderScale, 0.01f);
    ImGui::Checkbox("Render Point Lights", &editor.mRenderSettings.mValue.mShouldRenderPointLights);
    ImGui::End();

    // Inspector
    ImGui::Begin("Object Editor");
    mSceneObjectInspector.updateRender();
    ImGui::End();

    ImGui::Begin("Game");
    ImVec2 dim = ImGui::GetContentRegionAvail();
    ImGui::Image(editor.mOutputFramebuffer.getTextureID(0), { (float)editor.mOutputFramebuffer.getWidth(), (float)editor.mOutputFramebuffer.getHeight() }, { 0, 1 }, { 1, 0 });

    editor.mUIStyle.renderUIWindow();

    // Select object
    ImVec2 mousePos{ ImGui::GetMousePos() };
    ImVec2 windowPos{ ImGui::GetItemRectMin() };
    double mouseX = mousePos.x - windowPos.x;
    double mouseY = mousePos.y - windowPos.y;
    mouseY = editor.mOutputFramebuffer.getHeight() - 1 - mouseY;
    bool isCursorInGameWindow = mouseX > 0 && mouseX < editor.mOutputFramebuffer.getWidth() && mouseY > 0 && mouseY < editor.mOutputFramebuffer.getHeight();
    if (editor.mScene.getCamera().isCursorFree() && editor.mInputs.checkMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && isCursorInGameWindow) {
        SceneObject* selectedObjectPtr;
        std::array<float, 4> sample{ editor.mGeometryBuffers.samplePixel((int)mouseX, (int)mouseY, 4) };
        *((float*)(&selectedObjectPtr)) = sample[0];
        *(((float*)(&selectedObjectPtr)) + 1) = sample[1];
        Framebuffer::bind(outputFramebuffer);
        mSceneObjectInspector.toggleSelect(selectedObjectPtr);
    }

    ImGui::End();

    // Hierarchy
    ImGui::Begin("Hierarchy");
    renderSceneObjectHierarchyItem(editor.mScene.getRootObject().get(), editor.mScene);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return { (int)dim.x, (int)dim.y };
}

void EditorUI::renderSceneObjectHierarchyItem(SceneObject* object, Scene& scene) {
    if (!object)
        return;

    ImGui::PushID(object);
    if (ImGui::Button(object->getName().data())) {
        mSceneObjectInspector.toggleSelect(object);
    }
    ImGui::SameLine();
    if (ImGui::Button("+")) {
        object->addChild({ {0, 0, 0}, {1, 1 ,1}, {0, 0, 0} }, "New Object");
    }
    ImGui::SameLine();
    if (ImGui::Button("X")) {
        if (object->getParent()) {
            object->getParent()->destroyChild(object);
            ImGui::PopID();
            return;
        }
    }

    bool doChildren{ ImGui::CollapsingHeader("Children", object->getName() == "root" ? ImGuiTreeNodeFlags_DefaultOpen : 0)};
    ImGui::PopID();
    if (doChildren) {
        ImGui::Indent();
        for (const auto& child : object->getChildren()) {
            renderSceneObjectHierarchyItem(child.get(), scene);
        }
        ImGui::Unindent();
    }
}

void EditorUI::destroyUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}