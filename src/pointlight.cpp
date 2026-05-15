#include "pointlight.h"
#include <stdexcept>
#include <array>
#include "jsonhelpers.h"

PointLight::PointLight(const glm::vec3& colour)
	: Component{ ComponentTypes::pointLight }
	, mColour{ colour }
{}

std::unique_ptr<Component> PointLight::fromJSON(const JSON& json) {
	glm::vec3 colour{ JSONHelpers::toVec3(json.at("colour"))};
	return std::make_unique<PointLight>(colour);
}

JSON PointLight::toJSON() {
	JSON json;
	json["colour"] = JSONHelpers::fromVec3(mColour);
	return json;
}