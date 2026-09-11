#ifndef MOVE_3_HPP
#define MOVE_3_HPP

#include "script.hpp"
#include "nlohmann/json.hpp"

class Move3 : public Script {
public:
    Move3(const void* json);
    void create(EditableProperties& properties) override;
    void update(float deltaTime, SceneObject& sceneObject) override;

private:
    float mSpeed;
    bool mIsUp;
};

#endif