#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "shaderi.h"
#include "vertexarray.h"
#include "texture.h"
#include "app.h"

int main() {
    int screenWidth{ 1920 };
    int screenHeight{ 1080 };

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

    App app{ screenWidth, screenHeight, window };
    app.run();

    return 0;
}