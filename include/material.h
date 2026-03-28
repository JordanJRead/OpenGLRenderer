#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm/glm.hpp"
#include "texture.h"

struct Material {
	glm::vec3 mDiffuseColour;
	glm::vec3 mSpecularColour;
	std::array<size_t, (int)Texture::Type::max> mTextureIndices{};
};

#endif