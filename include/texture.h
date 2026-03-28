#ifndef TEXTURE_2_H
#define TEXTURE_2_H

#include <array>
#include <string>
#include "image.h"
#include "assimp/material.h"

namespace Texture {

	enum class Type {
		diffuse,
		specular,
		normal,
		max
	};

	extern std::array<std::string, (int)Type::max> names;
	extern std::array<aiTextureType, (int)Type::max> assimpTypes;
	extern std::array<Image*, (int)Type::max> defaultImages;

	class MemoryGuard {
	public:
		void loadDefaultTextures();
		~MemoryGuard();
	private:
		bool mHasLoaded{ false };
	};
	extern MemoryGuard memoryGuard;
}

#endif