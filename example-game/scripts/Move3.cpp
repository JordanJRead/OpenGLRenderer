#include "Move3.hpp"

#include "sceneobject.hpp"

Move3::Move3(const void* json) : Script{ "Move3" } {
    setJSONAndCreate(json ? *((const JSON*)json) : JSON::object());
}

void Move3::create(EditableProperties& properties) {
    mSpeed = properties.getOrCreate<EditableProperty::Type::double_type>("Speed");
    mIsUp = properties.getOrCreate<EditableProperty::Type::bool_type>("Up");
    // properties.getOrCreate<EditableProperty::Type::bool_type>("Test?");
}

void Move3::update(float deltaTime, SceneObject& sceneObject) {
    if (mIsUp)
        sceneObject.getTransform().mPosition.z += mSpeed * deltaTime;
    else
        sceneObject.getTransform().mPosition.x += mSpeed * deltaTime;
}
