#include "Move.hpp"

#include "sceneobject.hpp"

Move::Move(const JSON& json) : Script{ "Move" } {
    setJSONAndCreate(json);
}

void Move::create(EditableProperties& properties) {
    mSpeed = properties.getOrCreate<EditableProperty::Type::double_type>("Speed");
    mIsUp = properties.getOrCreate<EditableProperty::Type::bool_type>("Up");
}

void Move::update(float deltaTime, SceneObject& sceneObject) {
    if (mIsUp)
        sceneObject.getTransform().mPosition.y += mSpeed * deltaTime;
    else
        sceneObject.getTransform().mPosition.x += mSpeed * deltaTime;
}
