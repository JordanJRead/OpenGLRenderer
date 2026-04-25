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

json SceneObject::toJSON() const {
	json j;
	j["name"] = mName;
	j["transform"] = mTransform.toJSON();
	j["components"] = json::array();

	for (const auto& component : mComponents) {
		j["components"].push_back({ ComponentTypes::names[component.get()->getComponentType()], component.get()->toJSON() });
	}
	return j;
}

SceneObject::SceneObject(const json& j) : mTransform{ j.at("transform")} {
	mName = j.at("name");

	json components = j.at("components");
	for (auto item : components.items()) {
		std::string componentName{ item.key() };
		json componentJSON{ item.value() };
		ComponentTypes::Type type{ ComponentTypes::nameToType[componentName] };
		mComponents.push_back(ComponentTypes::fromJSON[(int)type](componentJSON));
	}
}