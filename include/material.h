#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm/glm.hpp"
#include "texturetypes.h"

struct Material {
	glm::vec3 mDiffuseColour;
	glm::vec3 mSpecularColour;
	float mSpecularExponent{ 32.0 };
	std::array<size_t, (int)TextureTypes::Type::max> mTextureIndices{};
};

#endif