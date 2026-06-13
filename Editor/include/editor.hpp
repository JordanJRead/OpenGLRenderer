#ifndef EDITOR_H
#define EDITOR_H

#include "camera.hpp"
#include "shaders/shadermesh.hpp"
#include "shaders/shaderpointlight.hpp"
#include "shaders/shaderpostprocess.hpp"
#include "shaders/shaderdeferred.hpp"
#include "transform.hpp"
#include "framebuffer.hpp"
#include "scene.hpp"
#include "vertexarrayscreen.hpp"
#include "GLFW/glfw3.h"
#include "editorui.hpp"
#include "rendersettings.hpp"
#include "inputs.hpp"
#include "openglbuffer.hpp"
#include <string_view>
#include "uistyle.hpp"

class Editor {
    friend class EditorUI;
public:
    Editor(int screenWidth, int screenHeight, GLFWwindow* window);
    void run();

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
    void saveToJSON(std::string_view fileName) const;
    void loadFromJSON(std::string_view fileName);

    Scene mScene;
    EditorUI mUI;
    ShaderPostProcess mPostProcessShader{ "assets/shaders/postprocess.vert", "assets/shaders/postprocess.frag" };
    ShaderDeferred mShaderDeferred{ "assets/shaders/deferred.vert", "assets/shaders/deferred.frag" };
    ShaderMesh mGeometryPassShader{ "assets/shaders/meshgeometry.vert", "assets/shaders/meshgeometry.frag" };
    ShaderPointLight mPointLightGeometryShader{ "assets/shaders/pointlightgeometry.vert", "assets/shaders/pointlightgeometry.frag" };
	float mPrevTime{ 0 };
    GLFWwindow* mWindow;
    Framebuffer mGeometryBuffers; // worldPos/sceneIndex, normal, diffuse, specular/exponent
    Framebuffer mOutputFramebuffer;
    VertexArrayScreen mScreenVertexArray;
    OpenGLBuffer<RenderSettings> mRenderSettings{ 2, BufferTypes::uniform };
    Inputs mInputs;
    UIStyle mUIStyle;
};

#endif