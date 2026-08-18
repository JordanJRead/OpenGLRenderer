#include "MoveDiag.hpp"

#include "sceneobject.hpp"

MoveDiag::MoveDiag(const void* json) : Script{ "MoveDiag" } {
    setJSONAndCreate(json ? *((const JSON*)json) : JSON::object());
}

void MoveDiag::create(EditableProperties& properties) {
    mSpeed = properties.getOrCreate<EditableProperty::Type::double_type>("Speed");
}

void MoveDiag::update(float deltaTime, SceneObject& sceneObject) {
    sceneObject.getTransform().mPosition.x += mSpeed * deltaTime;
    sceneObject.getTransform().mPosition.y += mSpeed * deltaTime;
}
