#ifndef APP_H
#define APP_H

#include "camera.h"
#include "shaderi.h"
#include "glm/glm.hpp"
#include "sceneobject.h"
#include "transform.h"

class App {
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
        Texture::memoryGuard.loadDefaultTextures();
	}

	void run() {

        //SceneObject object{ "assets/objects/sponza/sponza.obj", Transform{ {0, 0, 0}, { 0.1, -0.1, 0.1 }, { 0, 0, 0 } } };
        SceneObject object{ "assets/objects/plane/plane.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };
        //SceneObject object{ "assets/objects/testcube/testcube.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };
        //SceneObject object{ "assets/objects/scene/scene.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };
        float prevTime{ 0 };

        while (!glfwWindowShouldClose(mWindow)) {
            glfwPollEvents();
            float currentTime{ (float)glfwGetTime() };
            float deltaTime{ currentTime - prevTime };
            prevTime = currentTime;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            mCamera.update(mWindow, deltaTime);
            object.render(mScreenWidth, mScreenHeight, mCamera);
            if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE)) {
                glfwSetWindowShouldClose(mWindow, true);
            }

            glfwSwapBuffers(mWindow);
        }
	}

private:
	int mScreenWidth;
	int mScreenHeight;
	ShaderI mDefaultShader{ "assets/shaders/default.vert", "assets/shaders/default.frag" };
    ShaderI mDefaultNoTexShader{ "assets/shaders/defaultnotex.vert", "assets/shaders/defaultnotex.frag" };
	Camera mCamera{ glm::vec3{ 0, 0, 0 }, 100, 0.1 };
	float mPrevTime{ 0 };
    GLFWwindow* mWindow;
};

#endif