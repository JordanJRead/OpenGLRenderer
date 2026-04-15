#include "scene.h"
#include "transform.h"
#include "pointlight.h"
#include "componenttypes.h"
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

size_t Scene::addObject(const Transform& transform) {
	mObjects.emplace_back(transform);
	return mObjects.size() - 1;
}

SceneObject& Scene::getObject(size_t index) {
	return mObjects.at(index);
}

// TODO shader.render(), not model.render()
void Scene::render(int screenWidth, int screenHeight, const ShaderObject& shader, const Framebuffer* const framebuffer) const {
	for (const SceneObject& object : mObjects) {
		const Model* model{ object.getComponent<Model>() };
		if (model) {
			shader.setModelMatrix(object.getTransform());
			model->render(screenWidth, screenHeight, mCamera, shader, framebuffer);
		}
	}
}

void Scene::renderPointLights(int screenWidth, int screenHeight, const ShaderPointLight& shader, const Framebuffer* const framebuffer) const {
	if (framebuffer) {
		framebuffer->bind();
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	shader.setModelMatrix(mCamera, {});
	for (const SceneObject& object : mObjects) {
		const PointLight* pointLight{ object.getComponent<PointLight>() };
		if (pointLight) {
			shader.render(mSphereVertexArray, object.getTransform().mPosition, pointLight->mColour, glm::vec3{0.3,0.3,0.3});
		}
	}
}

void Scene::updatePointLights() {
	std::vector<float> data;
	for (const SceneObject& object : mObjects) {
		const PointLight* pointLight{ object.getComponent<PointLight>() };
		if (pointLight) {
			data.push_back(object.getTransform().mPosition.x);
			data.push_back(object.getTransform().mPosition.y);
			data.push_back(object.getTransform().mPosition.z);
			data.push_back(pointLight->mColour.x);
			data.push_back(pointLight->mColour.y);
			data.push_back(pointLight->mColour.z);
		}
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mPointLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
}