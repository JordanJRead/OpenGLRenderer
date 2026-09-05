#include "scene.hpp"
#include "directories.hpp"
#include "framebuffer.hpp"
#include "glad/glad.h"
#include "inputs.hpp"
#include "jsonhelpers.hpp"
#include "model.hpp"
#include "nlohmann/json.hpp"
#include "pointlight.hpp"
#include "rendersettings.hpp"
#include "shaders/shadermesh.hpp"
#include "shaders/shaderpointlight.hpp"
#include "transform.hpp"
#include <fstream>
#include <iostream>
#include <span>

Scene::Scene(const JSON& json)
	: mCamera{ glm::vec3{ 0, 0, 0 }, 100, 0.1 }
	, mRootObject{ std::make_unique<SceneObject>(json.at("rootObject"), nullptr) }
{
    // if (json.type() == JSON::value_t::array) {
    //     json = json[0]; // Don't know why this happens sometimes
    // }
    mAmbientLightColour = JSONHelpers::toVec3(json.at("ambientLightColour"));

    mCamera.loadJSONData(json.at("camera"));
    mDirectionalLight = DirectionalLight{ json.at("directionalLight") };

    mSphereVertexArray.create(
      "assets/objects/sphere/sphere.obj"); // TODO asset manager
}

void Scene::updateCameraData(GLFWwindow* window, const Inputs& inputs,
                             float deltaTime, float aspectRatio) {
    mCamera.update(window, inputs, deltaTime);
    mCameraDataBuffer.mValue = mCamera.getCameraRenderData(aspectRatio);
    mCameraDataBuffer.updateGPU();
}

void Scene::render(const ShaderMesh&        meshShader,
                   const ShaderPointLight&  pointLightShader,
                   const Framebuffer* const framebuffer,
                   const RenderSettings&    renderSettings,
                   SceneObject*             selectedObject) const {
    glDisable(GL_BLEND);
    renderSceneObject(*mRootObject, meshShader, pointLightShader, framebuffer,
                      renderSettings, selectedObject);
}

void Scene::renderSceneObject(const SceneObject&       sceneObject,
                              const ShaderMesh&        meshShader,
                              const ShaderPointLight&  pointLightShader,
                              const Framebuffer* const framebuffer,
                              const RenderSettings&    renderSettings,
                              const SceneObject*       selectedObject) const {
    const Model* model{ sceneObject.getComponent<Model>() };
    if (model && model->isValid()) {
        const std::span<const Mesh> meshes{ model->getMeshes() };
        for (const Mesh& mesh : meshes) {
            meshShader.render(mesh, *model, sceneObject,
                              &sceneObject == selectedObject, framebuffer,
                              sceneObject.getTransform());
        }
    }

    if (renderSettings.mShouldRenderPointLights) {
        const PointLight* pointLight{ sceneObject.getComponent<PointLight>() };
        if (pointLight) {
            pointLightShader.render(sceneObject, &sceneObject == selectedObject,
                                    mSphereVertexArray, framebuffer,
                                    sceneObject.getTransform().mPosition,
                                    pointLight->mColour);
        }
    }

    for (const auto& child : sceneObject.getChildren()) {
        renderSceneObject(*child, meshShader, pointLightShader, framebuffer,
                          renderSettings, selectedObject);
    }
}

void Scene::updatePointLights() {
    std::vector<float> data;
    getPointLightData(*mRootObject, data);
    mPointLightBuffer.mValue.mData = data;
    mPointLightBuffer.updateGPU();
}

void Scene::getPointLightData(const SceneObject&  sceneObject,
                              std::vector<float>& data) {
    const PointLight* pointLight{ sceneObject.getComponent<PointLight>() };
    if (pointLight) {
        data.push_back(sceneObject.getTransform().mPosition.x);
        data.push_back(sceneObject.getTransform().mPosition.y);
        data.push_back(sceneObject.getTransform().mPosition.z);
        data.push_back(pointLight->mColour.x);
        data.push_back(pointLight->mColour.y);
        data.push_back(pointLight->mColour.z);
    }
    for (const auto& child : sceneObject.getChildren()) {
        getPointLightData(*child, data);
    }
}

void Scene::updateAllObjectComponents() {
    updateObjectComponents(*mRootObject);
}

void Scene::updateObjectComponents(SceneObject& sceneObject) {
    for (auto& component : sceneObject.getComponents()) {
        component->updateProperties();
    }
    for (auto& child : sceneObject.getChildren()) {
        updateObjectComponents(*child);
    }
}

void Scene::saveToJSON() {
    JSON json;
    json["camera"]             = mCamera.toJSON();
    json["directionalLight"]   = mDirectionalLight.toJSON();
    json["ambientLightColour"] = JSONHelpers::fromVec3(mAmbientLightColour);

    json["rootObject"] = mRootObject->toJSON();
    std::ofstream file{ Directories::gameDirectoryPath
                        / Directories::sceneFileName };
    file << std::setw(1) << json;
    file.close();
}