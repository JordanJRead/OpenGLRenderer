#ifndef APP_H
#define APP_H

#include "camera.h"
#include "shaders/shaderobject.h"
#include "shaders/shaderpostprocess.h"
#include "shaders/shaderdeferred.h"
#include "glm/glm.hpp"
#include "sceneobject.h"
#include "transform.h"
#include "framebuffer.h"
#include "scene.h"
#include "pointlight.h"
#include "vertexarrayscreen.h"

class App {
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_E && action == GLFW_PRESS) {
            ((App*)glfwGetWindowUserPointer(window))->mDoNormalMapping = !((App*)glfwGetWindowUserPointer(window))->mDoNormalMapping;
            ((App*)glfwGetWindowUserPointer(window))->mGeometryPassShader.setNormalMapping(((App*)glfwGetWindowUserPointer(window))->mDoNormalMapping);
        }
    }
public:
	App(int screenWidth, int screenHeight, GLFWwindow* window)
        : mScreenWidth{ screenWidth }
        , mScreenHeight{ screenHeight }
        , mWindow{ window }
    {
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glClearColor(0.2, 0.2, 0.2, 1);
        TextureTypes::memoryGuard.loadDefaultTextures();
        glfwSetWindowUserPointer(mWindow, this);
        glfwSetKeyCallback(mWindow, keyCallback);
	}

	void run() {
        mScene.addObject("assets/objects/sponza/sponza.obj", Transform{ {0, 0, 0}, { 0.1, -0.1, 0.1 }, { 0, 0, 0 } });
        //mScene.addObject("assets/objects/breakfast_room/breakfast_room.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } });
        //mScene.addObject("assets/objects/bedroom/iscv2.obj", Transform{ {0, 0, 0}, { 0.3, 0.3, 0.3 }, { 270, 0, 0 } });
        //mScene.addObject("assets/objects/conference/conference.obj", Transform{ {0, 0, 0}, { 0.1, 0.1, 0.1 }, { 0, 0, 0 } });
        //mScene.addObject("assets/objects/sponza2/sponza.obj", Transform{ {0, 0, 0}, { 0.1, 0.1, 0.1 }, { 0, 0, 0 } });
        //mScene.addObject("assets/objects/fireplace_room/fireplace_room.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } });
        //mScene.addObject("assets/objects/gallery/gallery.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } });
        const int lightIndex = mScene.addPointLight({ {0, 1, 0}, {1, 1, 1} });
        //SceneObject object{ "assets/objects/plane/plane.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };
        //SceneObject object{ "assets/objects/testcube/testcube.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };
        //SceneObject object{ "assets/objects/scene/scene.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };
        float prevTime{ 0 };
        while (!glfwWindowShouldClose(mWindow)) {
            glfwPollEvents();
            float currentTime{ (float)glfwGetTime() };
            float deltaTime{ currentTime - prevTime };
            prevTime = currentTime;

            PointLight* pointLight = mScene.getPointLight(lightIndex);
            //pointLight->position = { 2 + glm::cos(glfwGetTime()), 2 + glm::sin(glfwGetTime()), -2 + glm::sin(glfwGetTime()) };
            mScene.updatePointLights();

            // Update
            mCamera.update(mWindow, deltaTime);

            // Render
            mFramebuffer.bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glDisable(GL_BLEND);
            mScene.render(mScreenWidth, mScreenHeight, mCamera, mGeometryPassShader, &mFramebuffer);
            if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE)) {
                glfwSetWindowShouldClose(mWindow, true);
            }

            mScene.renderPointLights(mScreenWidth, mScreenHeight, mCamera, mPointLightGeometryShader, &mFramebuffer);

            mShaderDeferred.render(mScreenVertexArray, mFramebuffer, mCamera.getPosition());

            glfwSwapBuffers(mWindow);
        }
	}

private:
	int mScreenWidth;
	int mScreenHeight;
    Scene mScene;
    ShaderPostProcess mPostProcessShader{ "assets/shaders/postprocess.vert", "assets/shaders/postprocess.frag" };
    ShaderDeferred mShaderDeferred{ "assets/shaders/deferred.vert", "assets/shaders/deferred.frag" };
    ShaderObject mDefaultShader{ "assets/shaders/default.vert", "assets/shaders/default.frag" };
    ShaderObject mDefaultNoTexShader{ "assets/shaders/defaultnotex.vert", "assets/shaders/defaultnotex.frag" };
    ShaderObject mGeometryPassShader{ "assets/shaders/geometrypass.vert", "assets/shaders/geometrypass.frag" };
    ShaderPointLight mPointLightGeometryShader{ "assets/shaders/pointlightgeometry.vert", "assets/shaders/pointlightgeometry.frag" };
	Camera mCamera{ glm::vec3{ 0, 0, 0 }, 100, 0.1 };
	float mPrevTime{ 0 };
    bool mDoNormalMapping = true;
    GLFWwindow* mWindow;
    Framebuffer mFramebuffer{ mScreenWidth, mScreenHeight, {GL_RGB32F, GL_RGB16F, GL_RGB16F, GL_RGBA16F } }; // worldPos, normal, diffuse, specular/exponent
    VertexArrayScreen mScreenVertexArray;
};

#endif