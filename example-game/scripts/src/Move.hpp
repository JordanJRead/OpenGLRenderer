#ifndef MOVE_HPP
#define MOVE_HPP

#include "script.hpp"
#include "nlohmann/json.hpp"

class Move : public Script {
public:
    Move(const JSON& json = JSON::object());
    void create(EditableProperties& properties) override;
    void update(float deltaTime, SceneObject& sceneObject) override;

private:
    float mSpeed;
    bool mIsUp;
};

#endif