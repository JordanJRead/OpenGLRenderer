#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "OpenGLObjects/BUF.h"
#include "camera.h"
#include "vertexarray.h"
#include "cameradatabuffer.h"
#include "directionallight.h"
#include "glm/glm.hpp"
#include "sceneobject.h"
#include <string_view>

class Framebuffer;
struct RenderSettings;
class ShaderMesh;
class ShaderPointLight;
class Transform;
class Inputs;

class Scene {
public:
	Scene(int screenWidth, int screenHeight, std::string_view jsonFilePath);
	size_t addObject(const Transform& transform, std::string_view name);
	SceneObject& getObject(size_t index);
	void render(const ShaderMesh& meshShader, const ShaderPointLight& pointLightShader, const Framebuffer* const framebuffer, const RenderSettings& renderSettings, int selectedObjectIndex) const;
	void updatePointLights();
	void updateCameraData(GLFWwindow* window, const Inputs& inputs, float deltaTime);
	bool isValidObjectIndex(int index) const { return index >= 0 && index < mObjects.size(); }

	Camera& getCamera() { return mCamera; }
	DirectionalLight& getDirectionalLight() { return mDirectionalLight; }
	glm::vec3& getAmbientLightColour() { return mAmbientLightColour; }
	
	void saveToJSON(std::string_view saveFilePath);

private:
	std::vector<SceneObject> mObjects;
	BUF mPointLightBuffer;
	VertexArray mSphereVertexArray;
	Camera mCamera;
	CameraDataBuffer mCameraDataBuffer;
	DirectionalLight mDirectionalLight{ {0, 1, 0}, {1, 1, 1} };
	glm::vec3 mAmbientLightColour{ 0.1, 0.1, 0.1 };
};

#endif