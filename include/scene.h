#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "OpenGLObjects/BUF.h"
#include "camera.h"
#include "vertexarray.h"
#include "camerarenderdata.h"
#include "directionallight.h"
#include "glm/glm.hpp"
#include "sceneobject.h"
#include <string_view>
#include "openglbuffer.h"
#include "pointlightdata.h"
#include <memory>

class Framebuffer;
class RenderSettings;
class ShaderMesh;
class ShaderPointLight;
class Transform;
class Inputs;

class Scene {
public:
	Scene(std::string_view jsonFilePath);

	void render(const ShaderMesh& meshShader, const ShaderPointLight& pointLightShader, const Framebuffer* const framebuffer, const RenderSettings& renderSettings, SceneObject* selectedObject) const;

	void updatePointLights();
	void updateCameraData(GLFWwindow* window, const Inputs& inputs, float deltaTime, float aspectRatio);

	const std::unique_ptr<SceneObject>& getRootObject() const { return mRootObject; }
	Camera& getCamera() { return mCamera; }
	DirectionalLight& getDirectionalLight() { return mDirectionalLight; }
	glm::vec3& getAmbientLightColour() { return mAmbientLightColour; }
	
	void saveToJSON(std::string_view saveFilePath);

private:
	std::unique_ptr<SceneObject> mRootObject;
	OpenGLBuffer<PointLightData> mPointLightBuffer{ 0, BufferTypes::ssbo };
	VertexArray mSphereVertexArray;
	Camera mCamera;

	OpenGLBuffer<CameraRenderData> mCameraDataBuffer{ 1, BufferTypes::uniform };
	DirectionalLight mDirectionalLight{ {0, 1, 0}, {1, 1, 1} };
	glm::vec3 mAmbientLightColour{ 0.1, 0.1, 0.1 };

	void renderSceneObject(const SceneObject* const sceneObject, const ShaderMesh& meshShader, const ShaderPointLight& pointLightShader, const Framebuffer* const framebuffer, const RenderSettings& renderSettings, SceneObject* selectedObject) const;
	void getPointLightData(const SceneObject* const sceneObject, std::vector<float>& data);
};

#endif