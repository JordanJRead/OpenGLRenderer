#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "glm/glm.hpp"
#include "component.hpp"
#include "nlohmann/json.hpp"

class PointLight : public Component {
public:
	PointLight(const JSON& json = JSON::object());
	glm::vec3 mColour;

private:
	void create(EditableProperties& properties) override;
};

#endif