#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "glm/glm.hpp"
#include "component.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class PointLight : public Component {
public:
	PointLight(const glm::vec3& colour);
	static std::unique_ptr<Component> fromJSON(const json& json);
	static ComponentTypes::Type getComponentType() { return ComponentTypes::pointLight; }
	void renderUIProperties() override {}
	json toJSON() override;
	glm::vec3 mColour;
};

#endif