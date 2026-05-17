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
#include "inputs.h"
#include <fstream>
#include "nlohmann/json.hpp"
#include "jsonhelpers.h"

Scene::Scene(std::string_view jsonFilePath)
	: mCamera{ glm::vec3{ 0, 0, 0 }, 100, 0.1 }
{
	std::ifstream file{ jsonFilePath.data() };
	if (file.is_open()) {
		JSON json{ JSON::parse(file) };
		file.close();
		mAmbientLightColour = JSONHelpers::toVec3(json.at("ambientLightColour"));

		mCamera.loadJSONData(json.at("camera"));
		mDirectionalLight = DirectionalLight{ json.at("directionalLight") };

		mRootObject = std::make_unique<SceneObject>(json.at("rootObject"), nullptr);
	}

	mSphereVertexArray.create("assets/objects/sphere/sphere.obj");
}

void Scene::updateCameraData(GLFWwindow* window, const Inputs& inputs, float deltaTime, float aspectRatio) {
	mCamera.update(window, inputs, deltaTime);
	mCameraDataBuffer.mValue = mCamera.getCameraRenderData(aspectRatio);
	mCameraDataBuffer.updateGPU();
}

void Scene::render(const ShaderMesh& meshShader, const ShaderPointLight& pointLightShader, const Framebuffer* const framebuffer, const RenderSettings& renderSettings, SceneObject* selectedObject) const {
	glDisable(GL_BLEND);
	renderSceneObject(mRootObject.get(), meshShader, pointLightShader, framebuffer, renderSettings, selectedObject);
}

void Scene::renderSceneObject(const SceneObject* const sceneObject, const ShaderMesh& meshShader, const ShaderPointLight& pointLightShader, const Framebuffer* const framebuffer, const RenderSettings& renderSettings, SceneObject* selectedObject) const {
	const Model* model{ sceneObject->getComponent<Model>() };
	if (model) {
		const std::span<const Mesh> meshes{ model->getMeshes() };
		for (const Mesh& mesh : meshes) {
			meshShader.render(mesh, *model, sceneObject, sceneObject == selectedObject, framebuffer, sceneObject->getTransform());
		}
	}

	if (renderSettings.mShouldRenderPointLights) {
		const PointLight* pointLight{ sceneObject->getComponent<PointLight>() };
		if (pointLight) {
			pointLightShader.render(sceneObject, sceneObject == selectedObject, mSphereVertexArray, framebuffer, sceneObject->getTransform().mPosition, pointLight->mColour);
		}
	}

	for (const auto& child : sceneObject->getChildren()) {
		renderSceneObject(child.get(), meshShader, pointLightShader, framebuffer, renderSettings, selectedObject);
	}
}

void Scene::updatePointLights() {
	std::vector<float> data;
	getPointLightData(mRootObject.get(), data);
	mPointLightBuffer.mValue.mData = data;
	mPointLightBuffer.updateGPU();
}

void Scene::getPointLightData(const SceneObject* const sceneObject, std::vector<float>& data) {
	const PointLight* pointLight{ sceneObject->getComponent<PointLight>() };
	if (pointLight) {
		data.push_back(sceneObject->getTransform().mPosition.x);
		data.push_back(sceneObject->getTransform().mPosition.y);
		data.push_back(sceneObject->getTransform().mPosition.z);
		data.push_back(pointLight->mColour.x);
		data.push_back(pointLight->mColour.y);
		data.push_back(pointLight->mColour.z);
	}
	for (const auto& child : sceneObject->getChildren()) {
		getPointLightData(child.get(), data);
	}
}

void Scene::saveToJSON(std::string_view saveFilePath) {
	JSON json;
	json["camera"] = mCamera.toJSON();
	json["directionalLight"] = mDirectionalLight.toJSON();
	json["ambientLightColour"] = JSONHelpers::fromVec3(mAmbientLightColour);

	json["rootObject"] = mRootObject->toJSON();
	std::ofstream file{ saveFilePath.data() };
	file << std::setw(1) << json;
	file.close();
}