#include "shaders/shaderi.h"
#include <iostream>
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_GLSL
#include "stb_include.h"

ShaderI::ShaderI(std::string_view vertPath, std::string_view fragPath) {
    
    // Read soruce code in
    char vertError[256];
    char* vertSource{ stb_include_file((char*)vertPath.data(), nullptr, std::string("assets/shaders").data(), vertError) };

    if (!vertSource) {
        std::cerr << "ERROR::SHADER::VERTEX::STB_INCLUDE_READ_FAILED\n" << vertPath << "\n" << vertError << "\n";
    }

    char fragError[256];
    char* fragSource{ stb_include_file((char*)fragPath.data(), nullptr, std::string("assets/shaders").data(), fragError) };

    if (!fragSource) {
        std::cerr << "ERROR::SHADER::VERTEX::STB_INCLUDE_READ_FAILED\n" << fragPath << "\n" << fragError << "\n";
    }

    // Compile vertex
    unsigned int vertShader{ glCreateShader(GL_VERTEX_SHADER) };
    glShaderSource(vertShader, 1, &vertSource, NULL);
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
    free(vertSource);
    free(fragSource);
}


ShaderI::~ShaderI() { glDeleteProgram(mID); }
void ShaderI::bind() const { glUseProgram(mID); }

void ShaderI::setMatrix4(std::string_view name, const glm::mat4& mat4) const {
    glUniformMatrix4fv(glGetUniformLocation(mID, name.data()), 1, false, glm::value_ptr(mat4));
}
void ShaderI::setVector3(std::string_view name, const glm::vec3& v) const {
    glUniform3fv(glGetUniformLocation(mID, name.data()), 1, glm::value_ptr(v));
}
void ShaderI::setVector2(std::string_view name, const glm::vec2& v) const {
    glUniform2fv(glGetUniformLocation(mID, name.data()), 1, glm::value_ptr(v));
}
void ShaderI::setFloat(std::string_view name, float f) const {
    glUniform1f(glGetUniformLocation(mID, name.data()), f);
}
void ShaderI::setUInt(std::string_view name, unsigned int n) const {
    glUniform1ui(glGetUniformLocation(mID, name.data()), n);
}
void ShaderI::setInt(std::string_view name, int n) const {
    glUniform1i(glGetUniformLocation(mID, name.data()), n);
}
void ShaderI::setBool(std::string_view name, bool b) const {
    glUniform1ui(glGetUniformLocation(mID, name.data()), b);
}