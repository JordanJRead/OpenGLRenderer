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

		for (const JSON& objectJSON : json.at("objects")) {
			mObjects.emplace_back(std::make_unique<SceneObject>(objectJSON, nullptr));
		}
	}

	mSphereVertexArray.create("assets/objects/sphere/sphere.obj");
}

void Scene::updateCameraData(GLFWwindow* window, const Inputs& inputs, float deltaTime, float aspectRatio) {
	mCamera.update(window, inputs, deltaTime);
	mCameraDataBuffer.mValue = mCamera.getCameraRenderData(aspectRatio);
	mCameraDataBuffer.updateGPU();
}

SceneObject* Scene::addObject(const Transform& transform, std::string_view name) {
	return mObjects.emplace_back(std::make_unique<SceneObject>(transform, name, nullptr)).get();
}

void Scene::render(const ShaderMesh& meshShader, const ShaderPointLight& pointLightShader, const Framebuffer* const framebuffer, const RenderSettings& renderSettings, SceneObject* selectedObject) const {
	glDisable(GL_BLEND);

	for (const std::unique_ptr<SceneObject>& objectUP : mObjects) {
		const SceneObject* const object{ objectUP.get() };
		const Model* model{ object->getComponent<Model>() };
		if (model) {
			const std::span<const Mesh> meshes{ model->getMeshes() };
			for (const Mesh& mesh : meshes) {
				meshShader.render(mesh, *model, object, object == selectedObject, framebuffer, object->getTransform());
			}
		}
	}

	if (renderSettings.mShouldRenderPointLights) {
		for (const std::unique_ptr<SceneObject>& objectUP : mObjects) {
			const SceneObject* const object{ objectUP.get() };
			const PointLight* pointLight{ object->getComponent<PointLight>() };
			if (pointLight) {
				pointLightShader.render(object, object == selectedObject, mSphereVertexArray, framebuffer, object->getTransform().mPosition, pointLight->mColour);
			}
		}
	}
}

void Scene::updatePointLights() {
	std::vector<float> data;
	for (const std::unique_ptr<SceneObject>& object : mObjects) {
		const PointLight* pointLight{ object->getComponent<PointLight>() };
		if (pointLight) {
			data.push_back(object->getTransform().mPosition.x);
			data.push_back(object->getTransform().mPosition.y);
			data.push_back(object->getTransform().mPosition.z);
			data.push_back(pointLight->mColour.x);
			data.push_back(pointLight->mColour.y);
			data.push_back(pointLight->mColour.z);
		}
	}
	mPointLightBuffer.mValue.mData = data;
	mPointLightBuffer.updateGPU();
}

void Scene::saveToJSON(std::string_view saveFilePath) {
	JSON json;
	json["camera"] = mCamera.toJSON();
	json["directionalLight"] = mDirectionalLight.toJSON();
	json["ambientLightColour"] = JSONHelpers::fromVec3(mAmbientLightColour);

	for (const std::unique_ptr<SceneObject>& object : mObjects) {
		json["objects"].push_back(object->toJSON());
	}
	std::ofstream file{ saveFilePath.data() };
	file << std::setw(1) << json;
	file.close();
}