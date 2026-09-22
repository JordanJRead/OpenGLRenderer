#include "Move.hpp"

#include "sceneobject.hpp"

Move::Move(const void* json) : Script{ "Move" } {
    setJSONAndCreate(json ? *((const JSON*)json) : JSON::object());
}

void Move::create(UIProperties& properties) {
    mSpeed = properties.getOrCreate<UIProperty::Type::double_type>("Speed");
    mIsUp = properties.getOrCreate<UIProperty::Type::bool_type>("Up");
    // properties.getOrCreate<UIProperty::Type::bool_type>("Test?");
}

void Move::update(float deltaTime, SceneObject& sceneObject) {
    if (mIsUp)
        sceneObject.getTransform().mPosition.y += mSpeed * deltaTime;
    else
        sceneObject.getTransform().mPosition.x += mSpeed * deltaTime;
}
