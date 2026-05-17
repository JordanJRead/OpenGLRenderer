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

JSON SceneObject::toJSON() const {
	JSON json;
	json["name"] = mName;
	json["transform"] = mTransform.toJSON();

	for (const auto& component : mComponents) {
		const std::string& componentName{ ComponentTypes::names[component.get()->getComponentType()] };
		json["components"][componentName] = component.get()->toJSON();
	}

	json["children"] = JSON::array();
	for (const auto& child : mChildren) {
		json["children"].push_back(child->toJSON());
	}
	return json;
}

SceneObject::SceneObject(const JSON& json) : mTransform{ json.at("transform")} {
	mName = json.at("name");

	JSON components = json.at("components");
	for (const auto& item : components.items()) {
		std::string componentName{ item.key() };
		JSON componentJSON{ item.value() };
		ComponentTypes::Type type{ ComponentTypes::nameToType[componentName] };
		mComponents.push_back(ComponentTypes::fromJSON[(int)type](componentJSON));
	}

	for (const auto& item : json.at("children")) {
		mChildren.emplace_back(std::make_unique<SceneObject>(item));
	}
}