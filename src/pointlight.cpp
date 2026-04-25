#include "pointlight.h"
#include <stdexcept>
#include <array>

PointLight::PointLight(const glm::vec3& colour)
	: Component{ ComponentTypes::pointLight }
	, mColour{ colour }
{}

std::unique_ptr<Component> PointLight::fromJSON(const JSON& json) {
	glm::vec3 colour;
	colour.r = json["colour"].at("r");
	colour.g = json["colour"].at("g");
	colour.b = json["colour"].at("b");
	return std::make_unique<PointLight>(colour);
}

JSON PointLight::toJSON() {
	JSON json;
	json["colour"]["r"] = mColour.r;
	json["colour"]["g"] = mColour.g;
	json["colour"]["b"] = mColour.b;
	return json;
}