#include "Move3.hpp"

#include "sceneobject.hpp"
#include "glm/glm.hpp"
#include <chrono>
#include <iostream>

Move3::Move3(const void* json) : Script{ "Move3" } {
    setJSONAndCreate(json ? *((const JSON*)json) : JSON::object());
}

void Move3::create(UIProperties& properties) {
    mSpeed = properties.getOrCreate<UIProperty::Type::double_type>("Speed");
    mIsUp = properties.getOrCreate<UIProperty::Type::bool_type>("Up");
    // mTest = properties.getOrCreate<UIProperty::Type::bool_type>("Test?");
}

void Move3::update(float deltaTime, SceneObject& sceneObject) {
    if (mIsUp)
        sceneObject.getTransform().mPosition.z += mSpeed * deltaTime;
    else
        sceneObject.getTransform().mPosition.x += mSpeed * deltaTime;
    // if (mTest) {
    //     double seconds = std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
    //     sceneObject.getTransform().mPosition.y = glm::sin(seconds * 2 * 3.14159);
    //     std::cout << seconds << "\n";
    // }
}
