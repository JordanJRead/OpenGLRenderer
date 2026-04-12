#include "scene.h"
#include "transform.h"
#include "pointlight.h"

#include "glad/glad.h"

Scene::Scene(int screenWidth, int screenHeight)
	: mCamera{ glm::vec3{ 0, 0, 0 }, 100, 0.1, screenWidth, screenHeight }
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mPointLightBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mPointLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 0, 0, GL_STATIC_DRAW);
	mSphereVertexArray.create("assets/objects/sphere/sphere.obj");
}

void Scene::updateCameraData(GLFWwindow* window, float deltaTime) {
	mCamera.update(window, deltaTime);
	mCameraDataBuffer.update(mCamera);
}

int Scene::addObject(const std::string& objPath, const Transform& transform) {
	mObjects.emplace_back(objPath, transform);
	return mObjects.size() - 1;
}

int Scene::addPointLight(const PointLight& pointLight) {
	mPointLights.push_back(pointLight);
	updatePointLights();
	return mPointLights.size() - 1;
}

PointLight* Scene::getPointLight(int index) {
	if (index >= 0 && index < mPointLights.size()) {
		return &(mPointLights[index]);
	}
	return nullptr;
}

void Scene::render(int screenWidth, int screenHeight, const ShaderObject& shader, const Framebuffer* const framebuffer) {
	for (const SceneObject& object : mObjects) {
		object.render(screenWidth, screenHeight, mCamera, shader, framebuffer);
	}
}

void Scene::renderPointLights(int screenWidth, int screenHeight, const ShaderPointLight& shader, const Framebuffer* const framebuffer) {
	if (framebuffer) {
		framebuffer->bind();
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	shader.setModelMatrix(mCamera, {});

	for (const PointLight& pointLight : mPointLights) {
		shader.render(mSphereVertexArray, pointLight, glm::vec3{ 0.3,0.3,0.3 });
	}
}

void Scene::updatePointLights() {
	std::vector<float> data;
	for (const PointLight& light : mPointLights) {
		data.push_back(light.position.x);
		data.push_back(light.position.y);
		data.push_back(light.position.z);
		data.push_back(light.colour.x);
		data.push_back(light.colour.y);
		data.push_back(light.colour.z);
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mPointLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
}