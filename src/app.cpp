#include "app.h"
#include "glm/glm.hpp"
#include "model.h"
#include "transform.h"
#include "pointlight.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void App::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    App* app = ((App*)glfwGetWindowUserPointer(window));

    if (!(ImGui::GetIO().WantCaptureKeyboard)) {
        if (action == GLFW_PRESS) {
            app->mInputs.registerKeyPress(key);
        }
    }
}

void App::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    App* app = ((App*)glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        app->mInputs.registerMouseButtonPress(button);
    }
}

App::App(int screenWidth, int screenHeight, GLFWwindow* window)
    : mScreenWidth{ screenWidth }
    , mScreenHeight{ screenHeight }
    , mWindow{ window }
    , mScene{ screenWidth, screenHeight, "scene.json" }
{
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

void App::run() {

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
        mScene.updateCameraData(mWindow, mInputs, deltaTime);

        // Render
        mGeometryBuffers.bind();
        glClearColor(0, 0, 0, -1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glDisable(GL_BLEND);
        mScene.render(mGeometryPassShader, mPointLightGeometryShader, &mGeometryBuffers, mRenderSettings);

        mShaderDeferred.render(mScreenVertexArray, nullptr, mGeometryBuffers, mScene.getDirectionalLight(), mScene.getAmbientLightColour());

        mEditor.updateRender(mScene, mWindow, mInputs, mGeometryBuffers);

        glfwSwapBuffers(mWindow);
    }
    mScene.saveToJSON("scene.json");
    mEditor.destroyUI();
    glfwTerminate();
}