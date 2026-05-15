#include "jsonhelpers.h"

JSON JSONHelpers::fromVec3(const glm::vec3& vec3) {
	JSON json;
	json["x"] = vec3.x;
	json["y"] = vec3.y;
	json["z"] = vec3.z;
	return json;
}

JSON JSONHelpers::fromVec4(const glm::vec4 & vec4) {
	JSON json;
	json["x"] = vec4.x;
	json["y"] = vec4.y;
	json["z"] = vec4.z;
	json["w"] = vec4.w;
	return json;
}

glm::vec3 JSONHelpers::toVec3(const JSON& json) {
	glm::vec3 vec3;
	vec3.x = json["x"];
	vec3.y = json["y"];
	vec3.z = json["z"];
	return vec3;
}

glm::vec4 JSONHelpers::toVec4(const JSON& json) {
	glm::vec4 vec4;
	vec4.x = json["x"];
	vec4.y = json["y"];
	vec4.z = json["z"];
	vec4.z = json["w"];
	return vec4;
}