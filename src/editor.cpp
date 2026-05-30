#include "editor.h"
#include "glm/glm.hpp"
#include "model.h"
#include "transform.h"
#include "pointlight.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "nlohmann/json.hpp"
#include <fstream>

void Editor::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    Editor* editor = ((Editor*)glfwGetWindowUserPointer(window));

    if (!(ImGui::GetIO().WantCaptureKeyboard)) {
        if (action == GLFW_PRESS) {
            editor->mInputs.registerKeyPress(key);
        }
    }
}

void Editor::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    Editor* editor = ((Editor*)glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        editor->mInputs.registerMouseButtonPress(button);
    }
}

Editor::Editor(int screenWidth, int screenHeight, GLFWwindow* window)
    : mWindow{ window }
    , mScene{ "scene.json" }
    , mGeometryBuffers{ screenWidth, screenHeight, {GL_RGBA32F, GL_RGB16F, GL_RGB16F, GL_RGBA16F, GL_RG32F }, {0, 0, 0, 0} } // worldPos, normal, diffuse, specular/exponent, objectPtr
    , mOutputFramebuffer{ screenWidth, screenHeight, {GL_RGBA8}, {0, 0, 0, 1} }
{
    loadFromJSON("editor.json");
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    TextureTypes::memoryGuard.loadDefaultTextures();
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetKeyCallback(mWindow, keyCallback);
    glfwSetMouseButtonCallback(mWindow, mouseCallback);
}

void Editor::run() {

    //mScene.addObject("assets/objects/breakfast_room/breakfast_room.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } });
    //mScene.addObject("assets/objects/bedroom/iscv2.obj", Transform{ {0, 0, 0}, { 0.3, 0.3, 0.3 }, { 270, 0, 0 } });
    //mScene.addObject("assets/objects/conference/conference.obj", Transform{ {0, 0, 0}, { 0.1, 0.1, 0.1 }, { 0, 0, 0 } });
    //mScene.addObject("assets/objects/sponza2/sponza.obj", Transform{ {0, 0, 0}, { 0.1, 0.1, 0.1 }, { 0, 0, 0 } });
    //mScene.addObject("assets/objects/fireplace_room/fireplace_room.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } });
    //mScene.addObject("assets/objects/gallery/gallery.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } });
    //SceneObject object{ "assets/objects/plane/plane.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };
    //SceneObject object{ "assets/objects/testcube/testcube.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };
    //SceneObject object{ "assets/objects/scene/scene.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };

    float prevTime{ 0 };
    while (!glfwWindowShouldClose(mWindow)) {

        mInputs.clear();
        if (!(ImGui::GetIO().WantCaptureKeyboard)) {
            mInputs.registerAllKeysDown(mWindow);
        }

        glfwPollEvents();
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(mWindow, true);
        }

        float currentTime{ (float)glfwGetTime() };
        float deltaTime{ currentTime - prevTime };
        prevTime = currentTime;

        //PointLight* pointLight = mScene.getPointLight(lightIndex);
        //pointLight->position = { 2 + glm::cos(glfwGetTime()), 2 + glm::sin(glfwGetTime()), -2 + glm::sin(glfwGetTime()) };
        mScene.updatePointLights();

        // Update
        mScene.updateCameraData(mWindow, mInputs, deltaTime, mOutputFramebuffer.getAspectRatio());

        // Render
        mGeometryBuffers.clear();
        mOutputFramebuffer.clear();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glDisable(GL_BLEND);
        mScene.render(mGeometryPassShader, mPointLightGeometryShader, &mGeometryBuffers, mRenderSettings.mValue, mUI.getSelectedObject());

        mShaderDeferred.render(mScreenVertexArray, &mOutputFramebuffer, mGeometryBuffers, mScene.getDirectionalLight(), mScene.getAmbientLightColour());

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glm::ivec2 newDim = mUI.updateRender(nullptr, *this);
        if (newDim.x != mOutputFramebuffer.getWidth() || newDim.y != mOutputFramebuffer.getHeight()) {
            mOutputFramebuffer.resize(newDim.x, newDim.y);
            mOutputFramebuffer.clear();
            mGeometryBuffers.resize(newDim.x, newDim.y);
            mGeometryBuffers.clear();
        }
        mRenderSettings.updateGPU();

        glfwSwapBuffers(mWindow);
    }
    mScene.saveToJSON("scene.json");
    saveToJSON("editor.json");
    mUI.destroyUI();
    glfwTerminate();
}

void Editor::saveToJSON(std::string_view fileName) const {
    JSON json;
    json["renderSettings"] = mRenderSettings.mValue.toJSON();
    json["uiStyle"] = mUIStyle.toJSON();
    std::ofstream file{ fileName.data() };
    file << std::setw(1) << json;
    file.close();
}

void Editor::loadFromJSON(std::string_view fileName) {
    std::ifstream file{ fileName.data() };
    if (file.is_open()) {
        JSON json{ JSON::parse(file) };
        file.close();
        mRenderSettings.mValue.fromJSON(json.at("renderSettings"));
        mUIStyle.fromJSON(json.at("uiStyle"));
    }
}