#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "pointlight.h"
#include "model.h"
#include <vector>
#include "OpenGLObjects/BUF.h"
#include "shaders/shaderobject.h"
#include "shaders/shaderpointlight.h"
#include "camera.h"
#include "framebuffer.h"
#include "vertexarray.h"
#include "cameradatabuffer.h"
#include "directionallight.h"
#include "glm/glm.hpp"
#include "sceneobject.h"
#include <optional>
#include "transform.h"

class Scene {
public:
	Scene(int screenWidth, int screenHeight);
	size_t addObject(const Transform& transform);
	SceneObject& getObject(size_t index);
	void render(int screenWidth, int screenHeight, const ShaderObject& shader, const Framebuffer* const framebuffer) const;
	void renderPointLights(int screenWidth, int screenHeight, const ShaderPointLight& shader, const Framebuffer* const framebuffer) const;
	void updatePointLights();
	void updateCameraData(GLFWwindow* window, float deltaTime);

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