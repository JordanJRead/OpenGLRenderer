#ifndef SCENE_H
#define SCENE_H

#include "OpenGLObjects/BUF.hpp"
#include "camera.hpp"
#include "camerarenderdata.hpp"
#include "directionallight.hpp"
#include "glm/glm.hpp"
#include "openglbuffer.hpp"
#include "pointlightdata.hpp"
#include "sceneobject.hpp"
#include "uniqueref.hpp"
#include "vertexarray.hpp"
#include <filesystem>
#include <memory>
#include <string_view>
#include <vector>

class Framebuffer;
class RenderSettings;
class ShaderMesh;
class ShaderPointLight;
class Transform;
class Inputs;

class Scene {
public:
    Scene(const JSON& json);

    void render(const ShaderMesh&        meshShader,
                const ShaderPointLight&  pointLightShader,
                const Framebuffer* const framebuffer,
                const RenderSettings&    renderSettings,
                SceneObject*             selectedObject) const;

    void updatePointLights();
    void updateCameraData(GLFWwindow* window, const Inputs& inputs,
                          float deltaTime, float aspectRatio);

    SceneObject& getRootObject() {
        return *mRootObject;
    }
    const SceneObject& getRootObject() const {
        return mRootObject;
    }
    Camera& getCamera() {
        return mCamera;
    }
    DirectionalLight& getDirectionalLight() {
        return mDirectionalLight;
    }
    glm::vec3& getAmbientLightColour() {
        return mAmbientLightColour;
    }

    void updateAllObjectComponents();

    void saveToJSON();

private:
    UniqueRef<SceneObject>       mRootObject;
    OpenGLBuffer<PointLightData> mPointLightBuffer{ 0, BufferTypes::ssbo };
    VertexArray                  mSphereVertexArray;
    Camera                       mCamera;

    OpenGLBuffer<CameraRenderData> mCameraDataBuffer{ 1, BufferTypes::uniform };
    DirectionalLight               mDirectionalLight{
                      { 0, 1, 0 },
                      { 1, 1, 1 }
    };
    glm::vec3 mAmbientLightColour{ 0.1, 0.1, 0.1 };

    void renderSceneObject(const SceneObject&       sceneObject,
                           const ShaderMesh&        meshShader,
                           const ShaderPointLight&  pointLightShader,
                           const Framebuffer* const framebuffer,
                           const RenderSettings&    renderSettings,
                           const SceneObject*       selectedObject) const;
    void getPointLightData(const SceneObject&  sceneObject,
                           std::vector<float>& data);
    void updateObjectComponents(SceneObject& sceneObject);
};

#endif