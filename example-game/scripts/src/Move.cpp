#include "Move.hpp"

#include "sceneobject.hpp"

Move::Move(const void* json) : Script{ "Move" } {
    setJSONAndCreate(json ? *((const JSON*)json) : JSON::object());
}

void Move::create(EditableProperties& properties) {
    mSpeed = properties.getOrCreate<EditableProperty::Type::double_type>("Speed");
    mIsUp = properties.getOrCreate<EditableProperty::Type::bool_type>("Up");
    // properties.getOrCreate<EditableProperty::Type::bool_type>("Test?");
}

void Move::update(float deltaTime, SceneObject& sceneObject) {
    if (mIsUp)
        sceneObject.getTransform().mPosition.y += mSpeed * deltaTime;
    else
        sceneObject.getTransform().mPosition.x += mSpeed * deltaTime;
}
