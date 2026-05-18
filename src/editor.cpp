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
#include "rendersettings.h"
#include "app.h"
#include "sceneobject.h"

Editor::Editor() {
    ImGuiStyle& style{ ImGui::GetStyle() };
    //style.Colors[ImGuiCol_Text] = ImVec4{ 0, 0, 1, 1 };
    //style.Colors[ImGuiCol_TextDisabled] = ImVec4{ 0, 0, 1, 1 };
    //style.Colors[ImGuiCol_WindowBg] = ImVec4{0, 0, 1, 1};
}

glm::ivec2 Editor::updateRender(const Framebuffer* const outputFramebuffer, App& app) {
    Framebuffer::bind(outputFramebuffer);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::Begin("Scene Lighting");
    ImGui::ColorEdit3("Sunlight Colour", (float*)&app.mScene.getDirectionalLight().mColour);
    ImGui::ColorEdit3("Ambient Colour", (float*)&app.mScene.getAmbientLightColour());
    ImGui::End();

    ImGui::Begin("Render Settings");
    ImGui::ColorEdit3("Highlight Colour", (float*)&app.mRenderSettings.mValue.mHighlightColour);
    ImGui::DragFloat("Point Light Size", &app.mRenderSettings.mValue.mPointLightRenderScale, 0.01f);
    ImGui::Checkbox("Render Point Lights", &app.mRenderSettings.mValue.mShouldRenderPointLights);
    ImGui::End();

    // Inspector
    ImGui::Begin("Object Editor");
    if (mSelectedObjectViewer.get()) {
        ImGui::PushID(mSelectedObjectViewer.get());
        ImGui::InputText("", &mSelectedObjectViewer.get()->getName());
        ImGui::PopID();
        mSelectedObjectViewer.get()->getTransform().renderUI();

        for (auto& component : mSelectedObjectViewer.get()->getComponents()) {
            component->renderUIProperties();
        }
    }

    else {
        ImGui::Text("No object selected");
    }
    ImGui::End();

    ImGui::Begin("Game");
    ImVec2 dim = ImGui::GetContentRegionAvail();
    ImGui::Image(app.mOutputFramebuffer.getTextureID(0), { (float)app.mOutputFramebuffer.getWidth(), (float)app.mOutputFramebuffer.getHeight() }, { 0, 1 }, { 1, 0 });

    app.mUIStyle.renderUIWindow();

    // Select object
    ImVec2 mousePos{ ImGui::GetMousePos() };
    ImVec2 windowPos{ ImGui::GetItemRectMin() };
    double mouseX = mousePos.x - windowPos.x;
    double mouseY = mousePos.y - windowPos.y;
    mouseY = app.mOutputFramebuffer.getHeight() - 1 - mouseY;
    bool isCursorInGameWindow = mouseX > 0 && mouseX < app.mOutputFramebuffer.getWidth() && mouseY > 0 && mouseY < app.mOutputFramebuffer.getHeight();
    if (app.mScene.getCamera().isCursorFree() && app.mInputs.checkMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && isCursorInGameWindow) {
        SceneObject* selectedObjectPtr;
        std::array<float, 4> sample{ app.mGeometryBuffers.samplePixel((int)mouseX, (int)mouseY, 4) };
        *((float*)(&selectedObjectPtr)) = sample[0];
        *(((float*)(&selectedObjectPtr)) + 1) = sample[1];
        Framebuffer::bind(outputFramebuffer);
        toggleSelect(selectedObjectPtr);
    }

    ImGui::End();

    // Hierarchy
    ImGui::Begin("Hierarchy");
    renderSceneObject(app.mScene.getRootObject().get(), app.mScene);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return { (int)dim.x, (int)dim.y };
}

void Editor::renderSceneObject(SceneObject* object, Scene& scene) {
    if (!object)
        return;

    ImGui::PushID(object);
    if (ImGui::Button(object->getName().data())) {
        toggleSelect(object);
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
            renderSceneObject(child.get(), scene);
        }
        ImGui::Unindent();
    }
}

void Editor::destroyUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Editor::toggleSelect(SceneObject * object) {
    if (object != mSelectedObjectViewer.get()) {
        mSelectedObjectViewer.lookAt(&object->mViewable);
    }
    else {
        mSelectedObjectViewer.lookAt(nullptr);
    }
}