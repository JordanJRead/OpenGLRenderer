#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "pointlight.h"
#include "sceneobject.h"
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

class Transform;

class Scene {
public:
	Scene(int screenWidth, int screenHeight);
	int addObject(const std::string& objPath, const Transform& transform);
	int addPointLight(const PointLight&);
	void render(int screenWidth, int screenHeight, const ShaderObject& shader, const Framebuffer* const framebuffer);
	void renderPointLights(int screenWidth, int screenHeight, const ShaderPointLight& shader, const Framebuffer* const framebuffer);
	void updatePointLights();
	void updateCameraData(GLFWwindow* window, float deltaTime);

	Camera& getCamera() { return mCamera; }
	DirectionalLight& getDirectionalLight() { return mDirectionalLight; };
	PointLight* getPointLight(int index);
	glm::vec3& getAmbientLightColour() { return mAmbientLightColour; }

private:
	std::vector<SceneObject> mObjects;
	std::vector<PointLight> mPointLights;
	BUF mPointLightBuffer;
	VertexArray mSphereVertexArray;
	Camera mCamera;
	CameraDataBuffer mCameraDataBuffer;
	DirectionalLight mDirectionalLight;
	glm::vec3 mAmbientLightColour{ 0.1, 0.1, 0.1 };
};

#endif