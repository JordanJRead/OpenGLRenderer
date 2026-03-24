#ifndef APP_H
#define APP_H

#include "camera.h"
#include "shaderi.h"
#include "glm/glm.hpp"

class App {
public:
	App(int screenWidth, int screenHeight, GLFWwindow* window)
        : mScreenWidth{ screenWidth }
        , mScreenHeight{ screenHeight }
        , mWindow{ window }
    {

	}

	void run() {
        VertexArray vertexArray;
        std::vector<float> vertexData{
            -0.5, -0.5, 0, 0,
             0.5, -0.5, 1, 0,
               0, 0.5, 0.5, 1
        };
        std::vector<unsigned int> indices{ 0, 1, 2 };
        std::vector<int> layout{ 2, 2 };
        vertexArray.create(vertexData, indices, layout);
        ShaderI helloShader{ "assets/shaders/hello.vert", "assets/shaders/hello.frag" };
        Texture texture{ "assets/images/test.png" };
        helloShader.use();
        helloShader.setInt("image", 0);
        texture.use(0);
        vertexArray.use();
        glClearColor(0.2, 0.2, 0.2, 1);

        while (!glfwWindowShouldClose(mWindow)) {
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDrawElements(GL_TRIANGLES, vertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
            glfwSwapBuffers(mWindow);
        }
	}

private:
	int mScreenWidth;
	int mScreenHeight;
	ShaderI mDefaultShader{ "assets/shaders/default.vert", "assets/shaders/default.frag" };
	Camera mCamera{ glm::vec3{ 0, 0, 0 }, 100, 0.1 };
	float mPrevTime{ 0 };
    GLFWwindow* mWindow;
};

#endif