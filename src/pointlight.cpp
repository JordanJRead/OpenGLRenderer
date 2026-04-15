#include "pointlight.h"

PointLight::PointLight(const glm::vec3& colour)
	: Component{ ComponentTypes::pointLight }
	, mColour{ colour }
{}