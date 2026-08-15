#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "OpenGLObjects/BUF.hpp"
#include "camera.hpp"
#include "vertexarray.hpp"
#include "camerarenderdata.hpp"
#include "directionallight.hpp"
#include "glm/glm.hpp"
#include "sceneobject.hpp"
#include <string_view>
#include "openglbuffer.hpp"
#include "pointlightdata.hpp"
#include <memory>
#include <filesystem>

class Framebuffer;
class RenderSettings;
class ShaderMesh;
class ShaderPointLight;
class Transform;
class Inputs;

class Scene {
public:
	Scene();

	void render(const ShaderMesh& meshShader, const ShaderPointLight& pointLightShader, const Framebuffer* const framebuffer, const RenderSettings& renderSettings, SceneObject* selectedObject) const;

	void updatePointLights();
	void updateCameraData(GLFWwindow* window, const Inputs& inputs, float deltaTime, float aspectRatio);

	const std::unique_ptr<SceneObject>& getRootObject() const { return mRootObject; }
	Camera& getCamera() { return mCamera; }
	DirectionalLight& getDirectionalLight() { return mDirectionalLight; }
	glm::vec3& getAmbientLightColour() { return mAmbientLightColour; }
	
	void saveToJSON();

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