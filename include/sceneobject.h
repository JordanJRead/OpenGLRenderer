#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <vector>
#include "component.h"
#include "transform.h"
#include <memory>
#include <optional>
#include <string>
#include <string_view>

class SceneObject {
public:
	SceneObject(const Transform& transform, std::string_view name, SceneObject* parent);
	SceneObject(const JSON& json, SceneObject* parent);

	JSON toJSON() const;

	void addComponent(std::unique_ptr<Component> component);
	void addChild(const Transform& transform, std::string_view name);
	
	template <typename T>
		requires std::is_base_of_v<Component, T>
	T* getComponent() {
		for (auto& component : mComponents) {
			if (component.get()->getComponentType() == T::getComponentType()) {
				return static_cast<T*>(component.get());
			}
		}
		return nullptr;
	}

	template <typename T>
		requires std::is_base_of_v<Component, T>
	const T* getComponent() const {
		for (const auto& component : mComponents) {
			if (component.get()->getComponentType() == T::getComponentType()) {
				return static_cast<const T*>(component.get());
			}
		}
		return nullptr;
	}

	const Transform& getTransform() const;
	Transform& getTransform();
	std::string_view getName() const { return mName; }
	std::string& getName() { return mName; }
	const std::vector<std::unique_ptr<SceneObject>>& getChildren() { return mChildren; }

private:
	std::vector<std::unique_ptr<Component>> mComponents;
	std::vector<std::unique_ptr<SceneObject>> mChildren;
	SceneObject* mParent;
	Transform mTransform;
	std::string mName;
};

#endif