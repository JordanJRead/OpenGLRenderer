#include "pointlight.h"
#include <stdexcept>
#include <array>
#include "jsonhelpers.h"

PointLight::PointLight(const glm::vec3& colour)
	: Component{ ComponentTypes::pointLight }
	, mColour{ colour }
{}

PointLight::PointLight(const JSON& metaJSON) : PointLight{ JSONHelpers::toVec3(metaJSON.at("colour")) } {
	mInitialMetaJSON = metaJSON;
}

//JSON PointLight::toMetaJSON() {
//	JSON json;
//	json["name"] = ComponentTypes::names[mComponentType];
//	json["data"]["colour"] = JSONHelpers::fromVec3(mColour);
//	return json;
//}