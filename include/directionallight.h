#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT

#include "glm/glm.hpp"

struct DirectionalLight {
	glm::vec3 mDirTo;
	glm::vec3 mColour;
};

#endif