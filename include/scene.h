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

class Framebuffer;
struct RenderSettings;
class ShaderMesh;
class ShaderPointLight;
class Transform;
class InputsJustPressed;

class Scene {
public:
	Scene(int screenWidth, int screenHeight);
	size_t addObject(const Transform& transform);
	SceneObject& getObject(size_t index);
	void render(const ShaderMesh& meshShader, const ShaderPointLight& pointLightShader, const Framebuffer* const framebuffer, const RenderSettings& renderSettings) const;
	void updatePointLights();
	void updateCameraData(GLFWwindow* window, const InputsJustPressed& inputs, float deltaTime);

	Camera& getCamera() { return mCamera; }
	DirectionalLight& getDirectionalLight() { return mDirectionalLight; }
	glm::vec3& getAmbientLightColour() { return mAmbientLightColour; }

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