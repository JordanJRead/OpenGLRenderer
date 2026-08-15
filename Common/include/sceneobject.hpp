#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <vector>
#include "component.hpp"
#include "transform.hpp"
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include "viewing.hpp"
#include "componentmanager.hpp"

class SceneObject {
public:
	SceneObject  (const Transform& transform, std::string_view name, SceneObject* parent);
	void addChild(const Transform& transform, std::string_view name);
	SceneObject  (const JSON& json, SceneObject* parent);
	void addChild(const JSON& json);

	JSON toJSON() const;

	void addComponent(std::unique_ptr<Component> component);
	void destroyChild(SceneObject* child);

	const Transform& getTransform() const;
	Transform& getTransform();
	std::string_view getName() const { return mName; }
	std::string& getName() { return mName; }
	SceneObject* getParent() const { return mParent; }
	const std::vector<std::unique_ptr<SceneObject>>& getChildren() const { return mChildren; }
	const std::vector<std::unique_ptr<Component>>& getComponents() const { return mComponents; }
	std::vector<std::unique_ptr<Component>>& getComponents() { return mComponents; }
	bool addComponentFromName(std::string_view componentTypeName);

	template <typename ComponentType>
		requires std::is_base_of_v<Component, ComponentType>
	bool addComponent() {
		if (getComponent<ComponentType>()) {
			return false;
		}
		mComponents.push_back(ComponentManager::instance().createComponent<ComponentType>());
		return true;
	}

	template <typename ComponentType>
		requires std::is_base_of_v<Component, ComponentType>
	ComponentType* getComponent() {
		for (const auto& component : mComponents) {
			ComponentType* derivedPointer{ dynamic_cast<ComponentType*>(component.get()) };
			if (derivedPointer) {
				return static_cast<const ComponentType*>(component.get());
			}
		}
		return nullptr;
	}

	template <typename ComponentType>
		requires std::is_base_of_v<Component, ComponentType>
	const ComponentType* getComponent() const {
		for (const auto& component : mComponents) {
			ComponentType* derivedPointer{ dynamic_cast<ComponentType*>(component.get()) };
			if (derivedPointer) {
				return static_cast<const ComponentType*>(component.get());
			}
		}
		return nullptr;
	}

	Viewable<SceneObject> mViewable{ this };

private:
	std::vector<std::unique_ptr<Component>> mComponents;
	std::vector<std::unique_ptr<SceneObject>> mChildren;
	SceneObject* mParent;
	Transform mTransform;
	std::string mName;
};

#endif