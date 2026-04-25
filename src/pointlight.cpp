#include "pointlight.h"
#include <stdexcept>
#include <array>

PointLight::PointLight(const glm::vec3& colour)
	: Component{ ComponentTypes::pointLight }
	, mColour{ colour }
{}

std::unique_ptr<Component> PointLight::fromJSON(const json& json) {
	glm::vec3 colour;
	colour.r = json["colour"].at("r");
	colour.g = json["colour"].at("g");
	colour.b = json["colour"].at("b");
	return std::make_unique<PointLight>(colour);
}

json PointLight::toJSON() {
	json j;
	j["colour"]["r"] = mColour.r;
	j["colour"]["g"] = mColour.g;
	j["colour"]["b"] = mColour.b;
	return j;
}