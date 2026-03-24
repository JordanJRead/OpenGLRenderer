#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm/glm.hpp"

struct Material {
	glm::vec3 mDiffuseColour;
	glm::vec3 mSpecularColour;
	size_t mDiffuseTextureIndex;
	size_t mSpecularTextureIndex;
};

#endif