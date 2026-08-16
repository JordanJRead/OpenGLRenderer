#include "scene.hpp"
#include "transform.hpp"
#include "pointlight.hpp"
#include "glad/glad.h"
#include "framebuffer.hpp"
#include "rendersettings.hpp"
#include "shaders/shadermesh.hpp"
#include "shaders/shaderpointlight.hpp"
#include <span>
#include "model.hpp"
#include "inputs.hpp"
#include <fstream>
#include "nlohmann/json.hpp"
#include "jsonhelpers.hpp"
#include "directories.hpp"
#include <iostream>

Scene::Scene()
	: mCamera{ glm::vec3{ 0, 0, 0 }, 100, 0.1 }
{
    std::cerr << "HERE1\n";
	std::ifstream file{ Directories::gameDirectoryPath / Directories::sceneFileName };
	if (file.is_open()) {
		JSON json = JSON::parse(file);
		if (json.type() == JSON::value_t::array) {
			json = json[0]; // Don't know why this happens sometimes
		}
    	std::cerr << "HERE2\n";
		file.close();
		std::cerr << json;
		mAmbientLightColour = JSONHelpers::toVec3(json.at("ambientLightColour"));
    	std::cerr << "HERE3\n";

		mCamera.loadJSONData(json.at("camera"));
    	std::cerr << "HERE4\n";
		mDirectionalLight = DirectionalLight{ json.at("directionalLight") };
    	std::cerr << "HERE5\n";
		mRootObject = std::make_unique<SceneObject>(json.at("rootObject"), nullptr);
    	std::cerr << "HERE6\n";
		std::cerr << "Done loading objects\n";
	}

	mSphereVertexArray.create("assets/objects/sphere/sphere.obj");
	std::cerr << "Done loading scene\n";
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
	if (model && model->isValid()) {
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

void Scene::saveToJSON() {
	JSON json;
	json["camera"] = mCamera.toJSON();
	json["directionalLight"] = mDirectionalLight.toJSON();
	json["ambientLightColour"] = JSONHelpers::fromVec3(mAmbientLightColour);

	json["rootObject"] = mRootObject->toJSON();
	std::ofstream file{ Directories::gameDirectoryPath / Directories::sceneFileName };
	file << std::setw(1) << json;
	file.close();
}