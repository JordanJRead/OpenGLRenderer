#ifndef APP_H
#define APP_H

#include "camera.h"
#include "shaders/shaderobject.h"
#include "shaders/shaderpostprocess.h"
#include "shaders/shaderdeferred.h"
#include "glm/glm.hpp"
#include "model.h"
#include "transform.h"
#include "framebuffer.h"
#include "scene.h"
#include "pointlight.h"
#include "vertexarrayscreen.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "cameradatabuffer.h"
#include "GLFW/glfw3.h"
#include "editor.h"

class App {
public:
    App(int screenWidth, int screenHeight, GLFWwindow* window);
    void run();

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	int mScreenWidth;
	int mScreenHeight;
    Scene mScene;
    Editor mEditor;
    ShaderPostProcess mPostProcessShader{ "assets/shaders/postprocess.vert", "assets/shaders/postprocess.frag" };
    ShaderDeferred mShaderDeferred{ "assets/shaders/deferred.vert", "assets/shaders/deferred.frag" };
    ShaderObject mGeometryPassShader{ "assets/shaders/geometrypass.vert", "assets/shaders/geometrypass.frag" };
    ShaderPointLight mPointLightGeometryShader{ "assets/shaders/pointlightgeometry.vert", "assets/shaders/pointlightgeometry.frag" };
	float mPrevTime{ 0 };
    GLFWwindow* mWindow;
    Framebuffer mGeometryBuffers{ mScreenWidth, mScreenHeight, {GL_RGB16F, GL_RGB16F, GL_RGB16F, GL_RGBA16F } }; // worldPos, normal, diffuse, specular/exponent
    VertexArrayScreen mScreenVertexArray;
};

#endif