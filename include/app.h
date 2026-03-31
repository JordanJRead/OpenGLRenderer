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
        TextureTypes::memoryGuard.loadDefaultTextures();

        mScreenVertexArray.create({
            -1, -1, 0, 0,
            1, -1, 1, 0,
            -1, 1, 0, 1,
            1, 1, 1 ,1
            }, {0, 1, 2, 1, 3, 2}, {2, 2});
	}

	void run() {

        SceneObject object{ "assets/objects/sponza/sponza.obj", Transform{ {0, 0, 0}, { 0.1, -0.1, 0.1 }, { 0, 0, 0 } } };
        //SceneObject object{ "assets/objects/plane/plane.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };
        //SceneObject object{ "assets/objects/testcube/testcube.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };
        //SceneObject object{ "assets/objects/scene/scene.obj", Transform{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } } };
        float prevTime{ 0 };
        while (!glfwWindowShouldClose(mWindow)) {
            glfwPollEvents();
            float currentTime{ (float)glfwGetTime() };
            float deltaTime{ currentTime - prevTime };
            prevTime = currentTime;

            // Update
            mCamera.update(mWindow, deltaTime);

            // Render
            mFramebuffer.bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            object.render(mScreenWidth, mScreenHeight, mCamera, mGeometryPassShader, &mFramebuffer);
            if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE)) {
                glfwSetWindowShouldClose(mWindow, true);
            }

            mShaderDeferred.setPerFrameInfo(mFramebuffer);
            mScreenVertexArray.bind();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDrawElements(GL_TRIANGLES, mScreenVertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(mWindow);
        }
	}

private:
	int mScreenWidth;
	int mScreenHeight;
    ShaderPostProcess mPostProcessShader{ "assets/shaders/postprocess.vert", "assets/shaders/postprocess.frag" };
    ShaderDeferred mShaderDeferred{ "assets/shaders/deferred.vert", "assets/shaders/deferred.frag" };
    ShaderObject mDefaultShader{ "assets/shaders/default.vert", "assets/shaders/default.frag" };
    ShaderObject mDefaultNoTexShader{ "assets/shaders/defaultnotex.vert", "assets/shaders/defaultnotex.frag" };
    ShaderObject mGeometryPassShader{ "assets/shaders/geometrypass.vert", "assets/shaders/geometrypass.frag" };
	Camera mCamera{ glm::vec3{ 0, 0, 0 }, 100, 0.1 };
	float mPrevTime{ 0 };
    GLFWwindow* mWindow;
    Framebuffer mFramebuffer{ mScreenWidth, mScreenHeight, {GL_RGB32F, GL_RGB16F, GL_RGB16 } }; // worldPos, normal, albedo
    VertexArray mScreenVertexArray;
};

#endif