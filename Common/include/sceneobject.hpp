#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "component.hpp"
#include "componentmanager.hpp"
#include "transform.hpp"
#include "uniqueref.hpp"
#include "viewing.hpp"
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

class SceneObject {
public:
    SceneObject(const Transform& transform, std::string_view name,
                SceneObject* parent);
    void addChild(const Transform& transform, std::string_view name);
    SceneObject(const JSON& json, SceneObject* parent);
    void addChild(const JSON& json);

    JSON toJSON() const;

    void destroyChild(SceneObject& child);

    const Transform& getTransform() const;
    Transform&       getTransform();
    std::string_view getName() const {
        return mName;
    }
    std::string& getName() {
        return mName;
    }
    SceneObject* getParent() const {
        return mParent;
    }
    std::vector<UniqueRef<SceneObject>>& getChildren() {
        return mChildren;
    }
    const std::vector<UniqueRef<SceneObject>>& getChildren() const {
        return mChildren;
    }
    std::vector<UniqueRef<Component>>& getComponents() {
        return mComponents;
    }
    const std::vector<UniqueRef<Component>>& getComponents() const {
        return mComponents;
    }
    bool addComponentFromName(std::string_view componentTypeName);

    template <typename ComponentType>
    requires std::is_base_of_v<Component, ComponentType>
    bool addComponent() {
        if (getComponent<ComponentType>()) {
            return false;
        }
        mComponents.emplace_back(
          ComponentManager::instance().createComponent<ComponentType>());
        return true;
    }

    SceneObject(const SceneObject&) = delete;

    template <typename ComponentType>
    requires std::is_base_of_v<Component, ComponentType>
    ComponentType* getComponent() {
        for (auto& component : mComponents) {
            ComponentType* derivedPointer{ dynamic_cast<ComponentType*>(
              component.get().get()) };
            if (derivedPointer) {
                return derivedPointer;
            }
        }
        return nullptr;
    }

    template <typename ComponentType>
    requires std::is_base_of_v<Component, ComponentType>
    const ComponentType* getComponent() const {
        for (const auto& component : mComponents) {
            const ComponentType* derivedPointer{
                dynamic_cast<const ComponentType*>(component.ptr())
            };
            if (derivedPointer) {
                return derivedPointer;
            }
        }
        return nullptr;
    }

    Viewable<SceneObject> mViewable{ this };

private:
    std::vector<UniqueRef<Component>>   mComponents;
    std::vector<UniqueRef<SceneObject>> mChildren;
    SceneObject*                        mParent;
    Transform                           mTransform;
    std::string                         mName;
};

#endif