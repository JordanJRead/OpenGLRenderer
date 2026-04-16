#include "app.h"
#include "glm/glm.hpp"
#include "model.h"
#include "transform.h"
#include "pointlight.h"
#include "GLFW/glfw3.h"

void App::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    App* app = ((App*)glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            app->mScene.getCamera().enableLooking();
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            app->mScene.getCamera().disableLooking();
        }
    }
}

App::App(int screenWidth, int screenHeight, GLFWwindow* window)
    : mScreenWidth{ screenWidth }
    , mScreenHeight{ screenHeight }
    , mWindow{ window }
    , mScene{ screenWidth, screenHeight }
{
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    TextureTypes::memoryGuard.loadDefaultTextures();
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetKeyCallback(mWindow, keyCallback);
}

void App::run() {
    mScene.getDirectionalLight().mDirTo = glm::vec3{ 0, 1, 0 };
    mScene.getDirectionalLight().mColour = glm::vec3{ 1, 1, 1 };

    const size_t lightIndex = mScene.addObject(Transform{ {0, 1, 0}, {1, 1, 1}, {0, 0, 0} });
    mScene.getObject(lightIndex).addComponent(std::make_unique<PointLight>(glm::vec3{ 1, 1, 1 }));

    const size_t modelIndex = mScene.addObject(Transform{ {0, 0, 0}, { 0.1, -0.1, 0.1 }, { 0, 0, 0 } });
    mScene.getObject(modelIndex).addComponent(std::make_unique<Model>("assets/objects/sponza/sponza.obj"));

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
        mScene.updateCameraData(mWindow, deltaTime);

        // Render
        mGeometryBuffers.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glDisable(GL_BLEND);
        mScene.render(mGeometryPassShader, mPointLightGeometryShader, &mGeometryBuffers, mRenderSettings);

        mShaderDeferred.render(mScreenVertexArray, nullptr, mGeometryBuffers, mScene.getDirectionalLight(), mScene.getAmbientLightColour());

        mEditor.updateRender(mScene);

        glfwSwapBuffers(mWindow);
    }
    mEditor.destroyUI();
    glfwTerminate();
}