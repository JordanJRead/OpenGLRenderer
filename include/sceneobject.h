#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <vector>
#include "component.h"
#include "transform.h"
#include <memory>
#include <optional>

class SceneObject {
public:
	SceneObject(const Transform& transform);
	void addComponent(std::unique_ptr<Component> component);
	
	template <typename T>
		requires std::is_base_of_v<Component, T>
	T* getComponent() {
		for (auto& component : mComponents) {
			if (component.get()->getComponentType() == T::getType()) {
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

private:
	std::vector<std::unique_ptr<Component>> mComponents;
	Transform mTransform;
};

#endif