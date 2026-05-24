#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "glm/glm.hpp"
#include "component.h"
#include <nlohmann/json.hpp>

class PointLight : public Component {
public:
	static ComponentTypes::Type getComponentType() { return ComponentTypes::pointLight; }

	PointLight(const JSON& json);
	PointLight();
	glm::vec3 mColour;

private:
	void create(EditableProperties& properties) override;
};

#endif