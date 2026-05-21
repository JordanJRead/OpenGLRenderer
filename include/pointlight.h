#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "glm/glm.hpp"
#include "component.h"
#include <nlohmann/json.hpp>

class PointLight : public Component {
public:
	PointLight(const glm::vec3& colour);
	PointLight(const JSON& json);
	static ComponentTypes::Type getComponentType() { return ComponentTypes::pointLight; }
	void renderUIProperties() override {}
	//JSON toMetaJSON() override;
	glm::vec3 mColour;
};

#endif