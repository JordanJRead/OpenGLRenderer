#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/glm.hpp"
#include "nlohmann/json.hpp"

using JSON = nlohmann::json;

class Transform {
public:
	Transform(const JSON& json);
	Transform(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotationDeg);
	glm::vec3 mPosition;
	glm::vec3 mScale;
	glm::vec3 mRotationDeg;

	glm::mat4 getModelMatrix() const;
	JSON toJSON() const;
};

#endif