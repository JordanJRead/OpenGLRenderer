#ifndef MOVE_DIAG_HPP
#define MOVE_DIAG_HPP

#include "script.hpp"
#include "nlohmann/json.hpp"

class MoveDiag : public Script {
public:
    MoveDiag(const void* json);
    void create(EditableProperties& properties) override;
    void update(float deltaTime, SceneObject& sceneObject) override;

private:
    float mSpeed;
};

#endif