#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <filesystem>
#include "editor.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "directories.hpp"
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <game_directory_path>\n";
        //return 1;
    }
    gGameDirectoryPath = "C:\\Users\\jorda\\source\\repos\\JordanJRead\\OpenGLRenderer\\example-game";// argv[1];

    // Create default settings
    if (!std::filesystem::exists(gGameDirectoryPath / gEditorSettingsFileName)) {
        std::filesystem::copy_file("./defaulteditorsettings.json", gGameDirectoryPath / gEditorSettingsFileName);
    }
    if (!std::filesystem::exists(gGameDirectoryPath / gImGuiIniFileName)) {
        std::filesystem::copy_file("./defaultimgui.ini", gGameDirectoryPath / gImGuiIniFileName);
    }

    int screenWidth{ 1920 };
    int screenHeight{ 1080 };

    // GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        return 1;
    }

    // GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return 1;
    }

    // IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io{ ImGui::GetIO() }; (void)io;

    std::string imGuiFilePathString = (gGameDirectoryPath / gImGuiIniFileName).string();
    io.IniFilename = imGuiFilePathString.c_str();
    
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    Editor editor{ screenWidth, screenHeight, window };
    editor.run();

    return 0;
}