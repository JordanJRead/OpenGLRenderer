#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include "glm/glm.hpp"
#include "nlohmann/json.hpp"

namespace JSONHelpers {

	JSON fromVec3(const glm::vec3& vec3);
	JSON fromVec4(const glm::vec4& vec4);
	glm::vec3 toVec3(const JSON& json);
	glm::vec4 toVec4(const JSON& json);
}

#endif