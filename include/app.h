#ifndef APP_H
#define APP_H

#include "camera.h"
#include "shaders/shadermesh.h"
#include "shaders/shaderpointlight.h"
#include "shaders/shaderpostprocess.h"
#include "shaders/shaderdeferred.h"
#include "transform.h"
#include "framebuffer.h"
#include "scene.h"
#include "vertexarrayscreen.h"
#include "GLFW/glfw3.h"
#include "editor.h"
#include "rendersettings.h"
#include "inputs.h"
#include "openglbuffer.h"
#include <string_view>
#include "uistyle.h"

class App {
    friend class Editor;
public:
    App(int screenWidth, int screenHeight, GLFWwindow* window);
    void run();

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
    void saveToJSON(std::string_view fileName) const;
    void loadFromJSON(std::string_view fileName);

    Scene mScene;
    Editor mEditor;
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