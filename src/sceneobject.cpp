#include "sceneobject.h"

SceneObject::SceneObject(const Transform& transform, std::string_view name) : mTransform{ transform }, mName{ name } {}

void SceneObject::addComponent(std::unique_ptr<Component> component) {
	mComponents.push_back(std::move(component));
}

const Transform& SceneObject::getTransform() const {
	return mTransform;
}

Transform& SceneObject::getTransform() {
	return mTransform;
}