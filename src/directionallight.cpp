#include "directionallight.h"

DirectionalLight::DirectionalLight(const glm::vec3& dirTo, const glm::vec3& colour)
	: mDirTo{ dirTo }
	, mColour{ colour }
{}

DirectionalLight::DirectionalLight(const JSON& json) {
	mDirTo.x = json["dirTo"].at("x");
	mDirTo.y = json["dirTo"].at("y");
	mDirTo.z = json["dirTo"].at("z");
	mColour.r = json["colour"].at("r");
	mColour.g = json["colour"].at("g");
	mColour.b = json["colour"].at("b");
}

JSON DirectionalLight::toJSON() const {
	JSON json;
	json["dirTo"]["x"] = mDirTo.x;
	json["dirTo"]["y"] = mDirTo.y;
	json["dirTo"]["z"] = mDirTo.z;
	json["colour"]["r"] = mColour.r;
	json["colour"]["g"] = mColour.g;
	json["colour"]["b"] = mColour.b;
}