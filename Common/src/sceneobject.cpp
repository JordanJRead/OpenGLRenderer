#include "sceneobject.hpp"
#include "componentmanager.hpp"
#include <algorithm>
#include <iostream>
#include <optional>

SceneObject::SceneObject(const Transform& transform, std::string_view name,
                         SceneObject* parent)
: mTransform{ transform }, mName{ name }, mParent{ parent } {
}

void SceneObject::destroyChild(SceneObject& child) {
    std::optional<size_t> foundIndex;
    for (size_t i{ 0 }; i < mChildren.size(); ++i) {
        if (mChildren[i].ptr() == &child) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex) {
        mChildren.erase(mChildren.begin() + *foundIndex);
    }
}

void SceneObject::addChild(const Transform& transform, std::string_view name) {
    mChildren.emplace_back(
      std::make_unique<SceneObject>(transform, name, this));
}

SceneObject::SceneObject(const JSON& json, SceneObject* parent)
: mTransform{ json.at("transform") }, mParent{ parent } {
    mName           = json.at("name");
    JSON components = json.at("components");
    for (const auto& item : components.items()) {
        std::string                componentName{ item.key() };
        JSON                       componentJSON = item.value();
        std::unique_ptr<Component> component{
            ComponentManager::instance().createComponentFromName(componentName,
                                                                 &componentJSON)
        };
        if (component) {
            mComponents.emplace_back(std::move(component));
        } else {
            std::cerr << "ERROR: Could not find component " << componentName
                      << "\n";
        }
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
    json["name"]      = mName;
    json["transform"] = mTransform.toJSON();

    json["components"] = JSON::object();
    for (const auto& component : mComponents) {
        json["components"][component->getName()] = component->toJSON();
    }

    json["children"] = JSON::array();
    for (const auto& child : mChildren) {
        json["children"].push_back(child->toJSON());
    }
    return json;
}

bool SceneObject::addComponentFromName(std::string_view componentTypeName) {
    for (const auto& component : mComponents) {
        if (componentTypeName == component->getName()) {
            return false;
        }
    }
    std::unique_ptr<Component> component{
        ComponentManager::instance().createComponentFromName(componentTypeName)
    };
    if (component) {
        mComponents.emplace_back(std::move(component));
    } else {
        std::cerr << "ERROR: Could not find component " << componentTypeName
                  << "\n";
    }
    return true;
}
