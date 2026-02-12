#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "shaderi.h"
#include "vertexarray.h"
#include "texture.h"

int main() {
    int screenWidth{ 800 };
    int screenHeight{ 600 };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return 1;
    }

    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        return 1;
    }

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

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, vertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
    }

    return 0;
}