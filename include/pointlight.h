#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "glm/glm.hpp"
#include "component.h"

class PointLight : public Component {
public:
	PointLight(const glm::vec3& colour);
	static ComponentTypes::Type getComponentType() { return ComponentTypes::pointLight; }
	void renderUIProperties() override {}
	glm::vec3 mColour;
};

#endif