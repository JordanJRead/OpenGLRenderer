#include "sceneobject.h"

SceneObject::SceneObject(const Transform& transform, std::string_view name, SceneObject* parent) : mTransform{ transform }, mName{ name }, mParent{ parent } {}

void SceneObject::addComponent(std::unique_ptr<Component> component) {
	mComponents.push_back(std::move(component));
}

void SceneObject::destroyChild(SceneObject* child) {
	for (auto& testChild : mChildren) {
		if (testChild.get() == child) {
			std::erase(mChildren, testChild);
			return;
		}
	}
}

void SceneObject::addChild(const Transform& transform, std::string_view name) {
	mChildren.emplace_back(std::make_unique<SceneObject>(transform, name, this));
}

SceneObject::SceneObject(const JSON& json, SceneObject* parent) : mTransform{ json.at("transform") }, mParent{ parent } {
	mName = json.at("name");

	JSON components = json.at("components");
	for (const auto& item : components.items()) {
		std::string componentName{ item.key() };
		JSON componentJSON{ item.value() };
		ComponentTypes::Type type{ ComponentTypes::nameToType[componentName] };
		mComponents.push_back(ComponentTypes::createFromJSON[(int)type](componentJSON));
	}

	for (const auto& item : json.at("children")) {
		mChildren.emplace_back(std::make_unique<SceneObject>(item, this));
	}
}

void SceneObject::addChild(const JSON& json) {
	mChildren.emplace_back(std::make_unique<SceneObject>(json, this));
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

	json["components"] = JSON::object();
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

bool SceneObject::addComponent(const std::string& componentName) {
	if (!ComponentTypes::nameToType.contains(componentName)) {
		return false;
	}
	ComponentTypes::Type type = ComponentTypes::nameToType[componentName];
	return addComponent(type);
}

bool SceneObject::addComponent(ComponentTypes::Type type) {
	for (const auto& component : mComponents) {
		if (component->getComponentType() == type) {
			return false;
		}
	}
	mComponents.push_back(ComponentTypes::createEmpty[(int)type]());
	return true;
}
