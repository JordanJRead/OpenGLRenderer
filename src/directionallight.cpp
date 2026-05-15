#include "directionallight.h"
#include "jsonhelpers.h"

DirectionalLight::DirectionalLight(const glm::vec3& dirTo, const glm::vec3& colour)
	: mDirTo{ dirTo }
	, mColour{ colour }
{}

DirectionalLight::DirectionalLight(const JSON& json) {
	mDirTo = JSONHelpers::toVec3(json.at("dirTo"));
	mColour = JSONHelpers::toVec3(json.at("colour"));
}

JSON DirectionalLight::toJSON() const {
	JSON json;
	json["dirTo"] = JSONHelpers::fromVec3(mDirTo);
	json["colour"] = JSONHelpers::fromVec3(mColour);
	return json;
}