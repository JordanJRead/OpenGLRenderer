#include <string>
#include <glad/glad.h>
#include "shaderi.h"
#include <sstream>
#include <iostream>
#include <fstream>
#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_GLSL
#include "stb_include.h"

ShaderI::ShaderI(const std::string& vertPath, const std::string& fragPath) {
    
    // Read soruce code in
    std::ifstream vertFS{ vertPath };
    std::stringstream vertSS;
    vertSS << vertFS.rdbuf();
    std::string vertexTemp{ vertSS.str() };
    const char* vertexSource{ vertexTemp.c_str() };

    std::ifstream fragFS{ fragPath };
    std::stringstream fragSS;
    fragSS << fragFS.rdbuf();
    std::string fragTemp{ fragSS.str() };
    const char* fragSource{ fragTemp.c_str() };

    // Compile vertex
    unsigned int vertShader{ glCreateShader(GL_VERTEX_SHADER) };
    glShaderSource(vertShader, 1, &vertexSource, NULL);
    glCompileShader(vertShader);

    int success;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertPath << "\n" << infoLog << "\n";
    }

    // Compile fragment
    unsigned int fragShader{ glCreateShader(GL_FRAGMENT_SHADER) };
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);

    success = 0;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragPath << infoLog << "\n";
    }

    // Program
    mID = glCreateProgram();
    glAttachShader(mID, vertShader);
    glAttachShader(mID, fragShader);
    glLinkProgram(mID);

    success = 0;
    glGetProgramiv(mID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(mID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << vertPath << " --> " << fragPath << "\n" << infoLog << "\n";
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}