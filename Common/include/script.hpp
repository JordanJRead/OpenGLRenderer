#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "component.hpp"
#include "nlohmann/json.hpp"
#include <string_view>

class SceneObject;

class Script : public Component {
public:
    Script(std::string_view scriptName, const JSON& json);
    virtual void update(float deltaTime, SceneObject& sceneObject) = 0;
};

#endif