#include "scene.h"
#include "transform.h"
#include "pointlight.h"
#include "glad/glad.h"
#include "framebuffer.h"
#include "rendersettings.h"
#include "shaders/shadermesh.h"
#include "shaders/shaderpointlight.h"
#include <span>
#include "model.h"
#include "inputsjustpressed.h"

Scene::Scene(int screenWidth, int screenHeight)
	: mCamera{ glm::vec3{ 0, 0, 0 }, 100, 0.1, screenWidth, screenHeight }
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mPointLightBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mPointLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 0, 0, GL_STATIC_DRAW);
	mSphereVertexArray.create("assets/objects/sphere/sphere.obj");
}

void Scene::updateCameraData(GLFWwindow* window, const InputsJustPressed& inputs, float deltaTime) {
	mCamera.update(window, inputs, deltaTime);
	mCameraDataBuffer.update(mCamera);
}

size_t Scene::addObject(const Transform& transform, std::string_view name) {
	mObjects.emplace_back(transform, name);
	return mObjects.size() - 1;
}

SceneObject& Scene::getObject(size_t index) {
	return mObjects.at(index);
}

void Scene::render(const ShaderMesh& meshShader, const ShaderPointLight& pointLightShader, const Framebuffer* const framebuffer, const RenderSettings& renderSettings) const {
	for (int objectI{ 0 }; objectI < mObjects.size(); ++objectI) {
		const SceneObject& object{ mObjects[objectI] };
		const Model* model{ object.getComponent<Model>() };
		if (model) {
			const std::span<const Mesh> meshes{ model->getMeshes() };
			for (const Mesh& mesh : meshes) {
				meshShader.render(mesh, *model, objectI, framebuffer, object.getTransform());
			}
		}
	}

	if (renderSettings.mShouldRenderPointLights) {
		for (const SceneObject& object : mObjects) {
			const PointLight* pointLight{ object.getComponent<PointLight>() };
			if (pointLight) {
				pointLightShader.render(mSphereVertexArray, framebuffer, object.getTransform().mPosition, renderSettings.mPointLightRenderScale, pointLight->mColour);
			}
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