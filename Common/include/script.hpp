#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "component.hpp"
#include <string_view>
#include "nlohmann/json.hpp"

class SceneObject;

class Script : public Component {
public:
    Script(std::string_view scriptName);
    virtual void update(float deltaTime, SceneObject& sceneObject) = 0;
};

#endif