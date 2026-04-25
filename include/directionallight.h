#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT

#include "glm/glm.hpp"
#include "nlohmann/json.hpp"

using JSON = nlohmann::json;

struct DirectionalLight {
	glm::vec3 mDirTo;
	glm::vec3 mColour;

	DirectionalLight(const glm::vec3& dirTo, const glm::vec3& colour);
	DirectionalLight(const JSON& json);
	JSON toJSON() const;
};

#endif